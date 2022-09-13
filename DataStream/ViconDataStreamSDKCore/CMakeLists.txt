find_package(Boost REQUIRED COMPONENTS date_time thread)

file(GLOB SOURCES "*.cpp")
add_library(ViconDataStreamSDKCore STATIC ${SOURCES})

target_include_directories(ViconDataStreamSDKCore
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/DataStream
)

set_target_properties(ViconDataStreamSDKCore PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(ViconDataStreamSDKCore
	PUBLIC
		StreamCommon
		ViconDataStreamSDKCoreUtils
		Boost::date_time
		Boost::thread
)
