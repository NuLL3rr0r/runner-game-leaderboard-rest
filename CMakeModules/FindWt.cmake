FIND_PATH ( WT_INCLUDE_DIR NAMES Wt PATHS /usr/include/ /usr/local/include/ )
FIND_LIBRARY ( WT_LIBRARY NAMES wt PATHS /usr/include/ /usr/local/include/ )
FIND_LIBRARY ( WT_FCGI_LIBRARY NAMES wtfcgi PATHS /usr/include/ /usr/local/include/ )
FIND_PATH ( WT_RESOURCES_DIR NAMES Wt/resources PATHS /usr/share/ /usr/local/share/ )


IF ( WT_INCLUDE_DIR AND WT_LIBRARY )
    SET ( WT_FOUND TRUE )
ENDIF (  )

IF ( WT_FCGI_LIBRARY )
    SET ( WT_FCGI_FOUND TRUE )
ENDIF (  )

IF ( WT_RESOURCES_DIR )
    SET ( WT_RESOURCES_FOUND TRUE )
ENDIF (  )


IF ( WT_FOUND )
    MESSAGE ( STATUS "Found Wt headers in ${WT_INCLUDE_DIR}" )
    MESSAGE ( STATUS "Found Wt library: ${WT_LIBRARY}" )
ELSE (  )
    IF ( WT_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find Wt" )
    ENDIF (  )
ENDIF (  )


IF ( WT_FCGI_FOUND )
    MESSAGE ( STATUS "Found Wt-FastCGI library: ${WT_FCGI_LIBRARY}" )
ELSE ( WT_FCGI_FOUND )
    IF ( WT_FCGI_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find Wt-FastCGI" )
    ENDIF ( WT_FCGI_FIND_REQUIRED )
ENDIF ( WT_FCGI_FOUND )


IF ( WT_RESOURCES_FOUND )
    SET ( WT_RESOURCES_DIR "${WT_RESOURCES_DIR}/Wt/resources" )
    MESSAGE ( STATUS "Found Wt resources: ${WT_RESOURCES_DIR}" )
ELSE ( WT_RESOURCES_FOUND )
    IF ( WT_RESOURCES_FIND_REQUIRED )
        MESSAGE ( FATAL_ERROR "Could not find Wt resources" )
    ENDIF ( WT_RESOURCES_FIND_REQUIRED )
ENDIF ( WT_RESOURCES_FOUND )


