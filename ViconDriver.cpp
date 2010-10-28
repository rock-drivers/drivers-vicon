#include "ViconDriver.hpp"
#include "Client.h"

#include <sstream>
#include <time.h>

using namespace vicon;
using namespace ViconDataStreamSDK::CPP;

namespace vicon {
    class DriverImpl
    {
    public:
	Client client;
    };
}


Driver::Driver()
    : impl( new DriverImpl() )
{
}

bool Driver::connect( const std::string& hostname, const unsigned int port )
{
    std::ostringstream host;
    host << hostname << ":" << port;

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
	return true;
    }
    else
    {
	// TODO dump error message
	return false;
    }
}

base::Time Driver::getTimestamp()
{
    // for now, just return the current time
    return base::Time::now();
}

Eigen::Transform3d Driver::getSegmentTransform( const std::string& subjectName, const std::string& segmentName )
{
    Output_GetSegmentGlobalTranslation trans = 
	impl->client.GetSegmentGlobalTranslation( subjectName, segmentName );

    Output_GetSegmentGlobalRotationQuaternion quat = 
	impl->client.GetSegmentGlobalRotationQuaternion( subjectName, segmentName );

    Eigen::Vector3d trans_m( trans.Translation[0], trans.Translation[1], trans.Translation[2] );

    // vicon data is in mm, but we prefer standard si units...
    Eigen::Transform3d result = Eigen::Translation3d( trans_m * 1e-3 ) * 
	Eigen::Quaterniond( quat.Rotation[0], quat.Rotation[1], quat.Rotation[2], quat.Rotation[3] );

    return result;
}

std::vector<Eigen::Vector3d> Driver::getUnlabeledMarkers()
{
    std::vector<Eigen::Vector3d> result;

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

