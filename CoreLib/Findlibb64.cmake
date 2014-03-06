FIND_PATH ( LIBB64_INCLUDE_DIR NAMES b64 PATHS /usr/include/ /usr/local/include/ )
FIND_LIBRARY ( LIBB64_LIBRARY NAMES b64 PATHS /usr/lib /usr/local/lib )


IF ( LIBB64_INCLUDE_DIR AND LIBB64_LIBRARY )
    SET ( LIBB64_FOUND TRUE )
ENDIF (  )


IF ( LIBB64_FOUND )
    MESSAGE ( STATUS "Found libb64 headers in ${LIBB64_INCLUDE_DIR}" )
    MESSAGE ( STATUS "Found libb64 library: ${LIBB64_LIBRARY}" )
ELSE (  )
    IF ( LIBB64_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find libb64" )
    ENDIF (  )
ENDIF (  )


