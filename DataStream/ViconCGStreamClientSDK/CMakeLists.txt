file(GLOB SOURCES "*.cpp")
add_library(ViconCGStreamClientSDK STATIC ${SOURCES})

target_include_directories(ViconCGStreamClientSDK
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/DataStream
)

target_link_libraries(ViconCGStreamClientSDK
	PUBLIC
		ViconCGStreamClient
)

set_target_properties(ViconCGStreamClientSDK PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)
