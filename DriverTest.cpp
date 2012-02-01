#include "ViconDriver.hpp"
#include <iostream>

using namespace std;

int main( int argc, char* argv[] )
{
    if( argc < 2 )
    {
	std::cout << "usage: DriverTest hostname subject segment" << std::endl;
	exit(0);
    }
    
    vicon::Driver driver;
    if( !driver.connect( std::string(argv[1]) ) )
    {
	std::cout << "could not connect to host " << argv[1] << std::endl;
	exit(0);
    }

    std::string subject( argv[2] );
    std::string segment( argv[3] );

    while(driver.getFrame())
    {
        bool is_there;
	std::cout << "got frame at " << driver.getTimestamp() << std::endl;
	std::cout << "transform: " << std::endl 
	    << driver.getSegmentTransform(subject, segment, is_there).matrix() << std::endl;
        if ( !is_there ) std::cout << "object is occluded!" << std::endl;
    }
}
