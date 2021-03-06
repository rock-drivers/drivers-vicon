#include "ViconDriver.hpp"
#include "Client.h"

#include <sstream>
#include <time.h>
#include <base-logging/Logging.hpp>

using namespace vicon;
using namespace ViconDataStreamSDK::CPP;

namespace vicon {
    class DriverImpl
    {
    public:
	Client client;
    };
}

int axesMap ( Direction::Enum axis ) {
    switch ( axis ) {
        case Direction::Up: return 3;
        case Direction::Down: return -3;
        case Direction::Left: return 2;
        case Direction::Right: return -2;
        case Direction::Forward: return 1;
        case Direction::Backward: return -1;
        default: return 0;
    }
}

Direction::Enum axesMap ( int axis ) {
    int axis_val = axis < 0 ? -axis : axis;
    switch ( axis_val ) {
        case 1:
            if ( axis > 0 ) return  Direction::Forward;
            else return Direction::Backward; 
        case 2:
            if ( axis > 0 ) return  Direction::Left;
            else return Direction::Right; 
        case 3:
            if ( axis > 0 ) return  Direction::Up;
            else return Direction::Down;
        default:
            return Direction::Forward;
    }
}

Driver::Driver()
    : impl( new DriverImpl() ), mLastResult(UNKNOWN)
{
}

bool Driver::connect( const std::string& hostname, const unsigned int port )
{
    std::ostringstream host;
    host << hostname << ":" << port;
    LOG_INFO_S << "connecting to " << host.str();

    impl->client.Connect( host.str() );

    if( isConnected() )
    {
	impl->client.EnableSegmentData();
	impl->client.EnableUnlabeledMarkerData();

	//impl->client.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
	impl->client.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
	// MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

	// Set the global up axis
	impl->client.SetAxisMapping( 
		Direction::Forward, 
		Direction::Left, 
		Direction::Up ); // Z-up

	return true;
    }
    else
	return false;
}

bool Driver::isConnected()
{
    return impl->client.IsConnected().Connected;
}

void Driver::disconnect()
{
    LOG_INFO_S << "disconnecting";
    impl->client.Disconnect();
}

bool Driver::getFrame( const base::Time& timeout )
{
    base::Time start = base::Time::now();
    Result::Enum result;
    while( ((result=impl->client.GetFrame().Result) == Result::NoFrame) && (start+timeout > base::Time::now()) )
    {
	const unsigned long wait_ms = 10;
#ifdef WIN32
	Sleep( wait_ms );
#else
	usleep( wait_ms );
#endif
    }
    
    if( result == Result::Success )
    {
        LOG_DEBUG_S << "Got Frame!";
	    return true;
    }
    else
    {
        if ( timeout.toSeconds() > 0 ) LOG_ERROR_S << "No Frame received!";
	    return false;
    }
}

base::Time Driver::getTimestamp()
{
    // for now, just return the current time
    return base::Time::now();
}

Eigen::Affine3d Driver::getSegmentTransform( const std::string& subjectName, const std::string& segmentName, bool& inFrame)
{
    
     // Get the segment name
    std::string SegmentName = impl->client.GetSubjectRootSegmentName(subjectName).SegmentName;
    
    LOG_DEBUG_S << "          Name: " << SegmentName;
	
    LOG_DEBUG_S << "get transform: " << subjectName << "." << segmentName;
    Output_GetSegmentGlobalTranslation trans = 
	impl->client.GetSegmentGlobalTranslation( subjectName, segmentName );

    Output_GetSegmentGlobalRotationQuaternion quat = 
	impl->client.GetSegmentGlobalRotationQuaternion( subjectName, segmentName );

    Eigen::Vector3d trans_m( trans.Translation[0], trans.Translation[1], trans.Translation[2] );

    // vicon data is in mm, but we prefer standard si units...
    Eigen::Affine3d result = Eigen::Translation3d( trans_m * 1e-3 ) * 
	Eigen::Quaterniond( quat.Rotation[3], quat.Rotation[0], quat.Rotation[1], quat.Rotation[2] );

    inFrame = !trans.Occluded;

    switch(trans.Result) {
        case(Result::InvalidSubjectName):
            mLastResult = INVALID_SUBJECT_NAME;
            break;
        case(Result::InvalidSegmentName):
            mLastResult = INVALID_SEGMENT_NAME;
            break;
        case(Result::Success):
            mLastResult = SUCCESS;
            break;
        default:
            mLastResult = UNKNOWN;
            break;
    }

    return result;
}

std::vector<base::Vector3d> Driver::getUnlabeledMarkers()
{
    std::vector<base::Vector3d> result;

    unsigned int UnlabeledMarkerCount = impl->client.GetUnlabeledMarkerCount().MarkerCount;
    for( unsigned int UnlabeledMarkerIndex = 0 ; UnlabeledMarkerIndex < UnlabeledMarkerCount ; ++UnlabeledMarkerIndex )
    { 
	Output_GetUnlabeledMarkerGlobalTranslation _Output_GetUnlabeledMarkerGlobalTranslation =
	    impl->client.GetUnlabeledMarkerGlobalTranslation( UnlabeledMarkerIndex );

        Eigen::Vector3d marker_pos( 
                    _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 0 ],
		    _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 1 ],
		    _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 2 ] );

	result.push_back( marker_pos * 1e-3 ); 
    }

    return result;
}


void Driver::getAxesDir( int& x_dir, int& y_dir, int& z_dir )
{
    Output_GetAxisMapping vicon_axes = impl->client.GetAxisMapping ();
    x_dir = axesMap( vicon_axes.XAxis );
    y_dir = axesMap( vicon_axes.YAxis );
    z_dir = axesMap( vicon_axes.ZAxis );
}

void Driver::setAxesDir( int x_dir, int y_dir, int z_dir )
{
    impl->client.SetAxisMapping( axesMap(x_dir), axesMap(y_dir), axesMap(z_dir) );

}
