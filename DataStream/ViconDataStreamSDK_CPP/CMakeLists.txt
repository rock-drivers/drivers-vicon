file(GLOB SOURCES "*.cpp")
add_library(ViconDataStreamSDK_CPP SHARED ${SOURCES})

target_include_directories(ViconDataStreamSDK_CPP
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include/ViconDataStreamSDK_CPP>
)

target_link_libraries(ViconDataStreamSDK_CPP
	PUBLIC
		ViconDataStreamSDKCore
		ViconCGStreamClientSDK
)

set_target_properties(ViconDataStreamSDK_CPP PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)

install(TARGETS ViconDataStreamSDK_CPP)

install(
	FILES
		IDataStreamClientBase.h
		DataStreamClient.h
		DataStreamRetimingClient.h
	DESTINATION
		include/ViconDataStreamSDK_CPP
)

# Install pkg-config file
configure_file(PkgConfig.in ViconDataStreamSDK_CPP.pc @ONLY)
install(
	FILES ${PROJECT_BINARY_DIR}/DataStream/ViconDataStreamSDK_CPP/ViconDataStreamSDK_CPP.pc
	DESTINATION lib/pkgconfig
)
