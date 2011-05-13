#ifndef __VICON_DRIVER_HPP__
#define __VICON_DRIVER_HPP__

#include <base/time.h>
#include <base/eigen.h>
#include <boost/shared_ptr.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>

namespace vicon
{
    class DriverImpl;

    /**
     * @file
     * @author Jakob Schwendner <jakob.schwendner@dfki.de>
     * 
     * @section DESCRIPTION
     *
     * This class is wrapping the driver for the Vicon DataStream SDK (currently
     * version 1.1) It is intended for easy acces to the device using Eigen2
     * types as interface types. 
     */
    class Driver
    {
    public:
	Driver();

	/** 
	 * Open a connection to the Vicon DataStream host
	 *
	 * @param hostname name of the network host
	 * @param port port numer
	 * @return true of the connection was successful, false otherwise
	 */
	bool connect( const std::string& hostname, const unsigned int port = 801 );

	/**
	 * @return true if there is a connection to the server
	 */
	bool isConnected();

	/**
	 * disconnect from the server
	 */
	void disconnect();

	/**
	 * Step to the next frame in the data stream. Always call this method
	 * before accessing any of the data methods.
	 *
	 * @param timeout time in milliseconds to wait for a new frame
	 * @result true if a frame has been received
	 */
	bool getFrame( const base::Time& timeout = base::Time::fromSeconds(1.0) );

	/** 
	 * @result the timestamp of the current frame
	 */
	base::Time getTimestamp();

	/** 
	 * Get the transformation which transforms the given segment from the
	 * local to the global coordinate frame.
	 *
	 * @param subjectName name of the subject
	 * @param segmentName name of the segment in the subject
	 * @result transformation from the segments local to the global reference frame
	 */
	Eigen::Affine3d getSegmentTransform( const std::string& subjectName, const std::string& segmentName );

	/** 
	 * Returns a list of unlabeled markers found by the system in global
	 * coordinates.
	 *
	 * @result a vector of unlabeled markers
	 */
	std::vector<base::Vector3d> getUnlabeledMarkers();

    private:
	boost::shared_ptr<DriverImpl> impl;
    };
}

#endif
