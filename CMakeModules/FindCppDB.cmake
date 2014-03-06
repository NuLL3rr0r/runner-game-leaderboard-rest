FIND_PATH ( CPPDB_INCLUDE_DIR NAMES cppdb PATHS /usr/include/ /usr/local/include/ )
FIND_LIBRARY ( CPPDB_LIBRARY NAMES cppdb PATHS /usr/lib /usr/local/lib )
FIND_LIBRARY ( CPPDB_SQLITE3_DRIVER NAMES cppdb_sqlite3 PATHS /usr/lib /usr/local/lib )


IF ( CPPDB_INCLUDE_DIR AND CPPDB_LIBRARY )
    SET ( CPPDB_FOUND TRUE )
ENDIF (  )

IF ( CPPDB_SQLITE3_DRIVER )
    SET ( CPPDB_SQLITE3_DRIVER_FOUND TRUE )
ENDIF (  )


IF ( CPPDB_FOUND )
    MESSAGE ( STATUS "Found CppDB headers in ${CPPDB_INCLUDE_DIR}" )
    MESSAGE ( STATUS "Found CppDB library: ${CPPDB_LIBRARY}" )
ELSE (  )
    IF ( CPPDB_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find CppDB." )
    ENDIF (  )
ENDIF (  )


IF ( CPPDB_SQLITE3_DRIVER_FOUND )
    MESSAGE ( STATUS "Found CppDB-SQLite3 driver: ${CPPDB_SQLITE3_DRIVER}" )
ELSE (  )
    IF ( CPPDB_SQLITE3_DRIVER_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find CppDB-SQLite3 driver." )
    ENDIF (  )
ENDIF (  )


