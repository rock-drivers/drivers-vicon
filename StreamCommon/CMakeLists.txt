file(GLOB SOURCES "*.cpp")
add_library(StreamCommon STATIC ${SOURCES})

target_include_directories(StreamCommon
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)

set_target_properties(StreamCommon PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)
