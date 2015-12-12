FIND_PATH ( CRYPTOPP_INCLUDE_DIR NAMES cryptopp PATHS /usr/include/ /usr/local/include/ )
FIND_LIBRARY ( CRYPTOPP_LIBRARY NAMES cryptopp PATHS /usr/lib /usr/local/lib )


IF ( CRYPTOPP_INCLUDE_DIR AND CRYPTOPP_LIBRARY )
    SET ( CRYPTOPP_FOUND TRUE )
ENDIF (  )


IF ( CRYPTOPP_FOUND )
    MESSAGE ( STATUS "Found Crypto++ headers in ${CRYPTOPP_INCLUDE_DIR}" )
    MESSAGE ( STATUS "Found Crypto++ library: ${CRYPTOPP_LIBRARY}" )
ELSE (  )
    IF ( CRYPTOPP_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find Crypto++" )
    ENDIF (  )
ENDIF (  )


