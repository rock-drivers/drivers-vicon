if( ${CMAKE_SYSTEM} MATCHES "Linux" )
    if( ${CMAKE_SIZEOF_VOID_P} EQUAL 4 )
	message("Found Linux32")
	set(VICON_LIB_FILE linux32/libViconDataStreamSDK.so)
    endif( ${CMAKE_SIZEOF_VOID_P} EQUAL 4 )

    if( ${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
	message("Found Linux64")
	set(VICON_LIB_FILE linux64/libViconDataStreamSDK.so)
    endif( ${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
endif( ${CMAKE_SYSTEM} MATCHES "Linux" )

if( ${CMAKE_SYSTEM} MATCHES "Windows" )
    if( ${CMAKE_SIZEOF_VOID_P} EQUAL 4 )
	message("Found Win32")
	message(FATAL_ERROR "Not yet supported. Please Implement me :).")
    endif( ${CMAKE_SIZEOF_VOID_P} EQUAL 4 )

    if( ${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
	message("Found Win64")
	message(FATAL_ERROR "Not yet supported. Please Implement me :).")
    endif( ${CMAKE_SIZEOF_VOID_P} EQUAL 8 )
endif( ${CMAKE_SYSTEM} MATCHES "Windows" )

file(COPY ${VICON_LIB_FILE} DESTINATION .)
install(FILES ${VICON_LIB_FILE} DESTINATION lib)
#add_library(ViconDataStreamSDK SHARED IMPORTED)

