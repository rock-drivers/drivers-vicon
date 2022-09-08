file(GLOB SOURCES "*.cpp")
add_library(ViconCGStreamClient STATIC ${SOURCES})

target_include_directories(ViconCGStreamClient
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/DataStream
)

target_link_libraries(ViconCGStreamClient
	PUBLIC
		StreamCommon
)

set_target_properties(ViconCGStreamClient PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)
