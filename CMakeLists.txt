# cmake file
project(vicon)
cmake_minimum_required(VERSION 2.8)

include(cmake/CommonSetup.cmake)

find_package(Eigen2 REQUIRED)
include_directories(${EIGEN2_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(lib)
link_directories(${PROJECT_BINARY_DIR}/lib)

add_executable(ViconDump ViconDump.cpp)
target_link_libraries(ViconDump ViconDataStreamSDK)

