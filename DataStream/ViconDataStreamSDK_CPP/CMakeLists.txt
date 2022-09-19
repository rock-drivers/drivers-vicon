include(GNUInstallDirs)

file(GLOB SOURCES "*.cpp")
set(HEADERS
	IDataStreamClientBase.h
	DataStreamClient.h
	DataStreamRetimingClient.h
)

add_library(ViconDataStreamSDK_CPP SHARED ${SOURCES})

target_include_directories(ViconDataStreamSDK_CPP
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ViconDataStreamSDK_CPP>
)

target_link_libraries(ViconDataStreamSDK_CPP
	PUBLIC
		ViconDataStreamSDKCore
		ViconCGStreamClientSDK
)

set_target_properties(ViconDataStreamSDK_CPP PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	PUBLIC_HEADER "${HEADERS}"
)

install(TARGETS ViconDataStreamSDK_CPP
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ViconDataStreamSDK_CPP
	LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install pkg-config file
configure_file(PkgConfig.in ViconDataStreamSDK_CPP.pc @ONLY)
install(
	FILES ${PROJECT_BINARY_DIR}/DataStream/ViconDataStreamSDK_CPP/ViconDataStreamSDK_CPP.pc
	DESTINATION lib/pkgconfig
)

