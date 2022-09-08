file(GLOB SOURCES "*.cpp")
add_library(ViconDataStreamSDKCoreUtils STATIC ${SOURCES})

target_include_directories(ViconDataStreamSDKCoreUtils
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/DataStream
)

set_target_properties(ViconDataStreamSDKCoreUtils PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)
