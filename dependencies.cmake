SET ( Boost_FIND_REQUIRED TRUE )
SET ( Boost_USE_MULTITHREADED ON )
SET ( Boost_USE_STATIC_LIBS OFF )
SET ( Boost_USE_STATIC_RUNTIME OFF )
SET ( CPPDB_FIND_REQUIRED TRUE )
SET ( CPPDB_SQLITE3_DRIVER_FIND_REQUIRED TRUE )
SET ( CRYPTOPP_FIND_REQUIRED TRUE )
SET ( LIBB64_FIND_REQUIRED TRUE )
SET ( SQLITE3_FIND_REQUIRED TRUE )
SET ( Threads_FIND_REQUIRED TRUE )
SET ( WT_FIND_REQUIRED TRUE )
SET ( WT_FCGI_FIND_REQUIRED TRUE )
SET ( WT_RESOURCES_FIND_REQUIRED TRUE )



### Boost ###
FIND_PACKAGE (Boost 1.52.0
    COMPONENTS
        chrono date_time filesystem regex thread system
    )
INCLUDE_DIRECTORIES ( SYSTEM ${Boost_INCLUDE_DIRS} )
LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )


### CppDB ###
FIND_PACKAGE ( CppDB )
INCLUDE_DIRECTORIES ( SYSTEM ${CPPDB_INCLUDE_DIR} )
LINK_DIRECTORIES ( ${CPPDB_LIBRARY} )


### Crypto++ ###
FIND_PACKAGE ( CryptoPP )
INCLUDE_DIRECTORIES ( SYSTEM ${CRYPTOPP_INCLUDE_DIR} )
LINK_DIRECTORIES ( ${CRYPTOPP_LIBRARY} )


### libb64 ###
FIND_PACKAGE ( libb64 )
INCLUDE_DIRECTORIES ( SYSTEM ${LIBB64_INCLUDE_DIR} )
LINK_DIRECTORIES ( ${LIBB64_LIBRARY} )


### SQLite 3 ###
FIND_PACKAGE ( SQLite3 )
INCLUDE_DIRECTORIES ( SYSTEM ${SQLITE3_INCLUDE_DIR} )
LINK_DIRECTORIES ( ${SQLITE3_LIBRARY} )


### Threads ###
FIND_PACKAGE ( Threads )
IF ( NOT Threads_FOUND )
    IF ( Threads_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find Threads" )
    ENDIF (  )
ENDIF (  )


### Wt ###
FIND_PACKAGE ( Wt )
INCLUDE_DIRECTORIES ( SYSTEM ${WT_INCLUDE_DIR} )
LINK_DIRECTORIES ( ${WT_LIBRARY} )
LINK_DIRECTORIES ( ${WT_FCGI_LIBRARY} )


