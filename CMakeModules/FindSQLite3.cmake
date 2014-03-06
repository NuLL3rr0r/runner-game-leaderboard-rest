FIND_PATH ( SQLITE3_INCLUDE_DIR NAMES sqlite3.h PATHS /usr/include/ /usr/local/include/ )
FIND_LIBRARY ( SQLITE3_LIBRARY NAMES sqlite3 PATHS /usr/lib /usr/local/lib ) 


IF ( SQLITE3_INCLUDE_DIR AND SQLITE3_LIBRARY )
    SET ( SQLITE3_FOUND TRUE )
ENDIF (  )


IF ( SQLITE3_FOUND )
    MESSAGE ( STATUS "Found SQLite3 headers in ${SQLITE3_INCLUDE_DIR}" )
    MESSAGE ( STATUS "Found SQLite3 library: ${SQLITE3_LIBRARY}" )
ELSE (  )
    IF ( SQLITE3_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find SQLite3." )
    ENDIF (  )
ENDIF (  )


