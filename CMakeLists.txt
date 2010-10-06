# cmake file
project(vicon)
cmake_minimum_required(VERSION 2.8)

include(cmake/CommonSetup.cmake)
include(FindPkgConfig)

#include_directories(${PROJECT_SOURCE_DIR})

find_package(Eigen2 REQUIRED)
include_directories(${EIGEN2_INCLUDE_DIR})

pkg_check_modules(DFKI REQUIRED "base-types")
include_directories(${DFKI_INCLUDE_DIRS})
link_directories(${DFKI_LIBRARY_DIRS})

add_subdirectory(lib)
link_directories(${PROJECT_BINARY_DIR}/lib)

add_executable(ViconDump ViconDump.cpp)
target_link_libraries(ViconDump ViconDataStreamSDK)

add_library(vicon ViconDriver.cpp)
target_link_libraries(vicon ViconDataStreamSDK)
install(TARGETS vicon
    LIBRARY DESTINATION lib 
    ARCHIVE DESTINATION lib )
set_target_properties(vicon PROPERTIES COMPILE_FLAGS -fPIC)
install(FILES ViconDriver.hpp DESTINATION include)

add_executable(DriverTest DriverTest.cpp)
target_link_libraries(DriverTest vicon ViconDataStreamSDK)

