# cmake file
project(vicon)
cmake_minimum_required(VERSION 2.8)

add_definitions(-DBASE_LOG_NAMESPACE=${PROJECT_NAME})
add_definitions(-DBASE_LOG_DEBUG)

include(cmake/CommonSetup.cmake)
include(FindPkgConfig)

pkg_check_modules(DFKI REQUIRED "base-types")
include_directories(${DFKI_INCLUDE_DIRS})
link_directories(${DFKI_LIBRARY_DIRS})

pkg_check_modules(BASELIB REQUIRED "base-lib")
include_directories(${BASELIB_INCLUDE_DIRS})
link_directories(${BASELIB_LIBRARY_DIRS})

add_subdirectory(lib)
link_directories(${PROJECT_BINARY_DIR}/lib)

add_executable(ViconDump ViconDump.cpp)
target_link_libraries(ViconDump ViconDataStreamSDK)

add_library(vicon SHARED ViconDriver.cpp)
target_link_libraries(vicon ViconDataStreamSDK ${BASELIB_LIBRARIES})
install(TARGETS vicon
    LIBRARY DESTINATION lib 
    ARCHIVE DESTINATION lib )
install(FILES ViconDriver.hpp DESTINATION include)

add_executable(DriverTest DriverTest.cpp)
target_link_libraries(DriverTest vicon ViconDataStreamSDK)

