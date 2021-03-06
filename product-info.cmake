SET ( ROSSNBOSS_PRODUCT_COMPANY_NAME "CompanyName" )
SET ( ROSSNBOSS_PRODUCT_COPYRIGHT_HOLDER "CopyrightHolder" )
SET ( ROSSNBOSS_PRODUCT_COPYRIGHT_YEAR "2014" )
SET ( ROSSNBOSS_PRODUCT_COPYRIGHT "(C) ${ROSSNBOSS_PRODUCT_COPYRIGHT_YEAR} ${ROSSNBOSS_PRODUCT_COPYRIGHT_HOLDER}" )

SET ( ROSSNBOSS_REST_PRODUCT_INTERNAL_NAME "RossNBossREST" )
SET ( ROSSNBOSS_REST_PRODUCT_NAME "Ross & Boss RESTful Web Service" )
SET ( ROSSNBOSS_REST_PRODUCT_VERSION_MAJOR "0" )
SET ( ROSSNBOSS_REST_PRODUCT_VERSION_MINOR "1" )
SET ( ROSSNBOSS_REST_PRODUCT_VERSION_PATCH "0" )
SET ( ROSSNBOSS_REST_PRODUCT_DESCRIPTION "This is the RESTful web service for Ross & Boss" )

IF ( EXISTS "${PROJECT_SOURCE_DIR}/.git" )
    EXEC_PROGRAM ( ${GIT_EXECUTABLE}
        ARGS "--git-dir=\"${PROJECT_SOURCE_DIR}/.git\" --work-tree=\"${PROJECT_SOURCE_DIR}\" describe --always"
        OUTPUT_VARIABLE GIT_SHORT_REVISION_HASH )
    SET ( ROSSNBOSS_REST_PRODUCT_VERSION_REVISION ${GIT_SHORT_REVISION_HASH} )
ELSE (  )
    SET ( ROSSNBOSS_REST_PRODUCT_VERSION_REVISION "{non-git}" )
ENDIF (  )

SET ( ROSSNBOSS_REST_PRODUCT_VERSION "${ROSSNBOSS_REST_PRODUCT_VERSION_MAJOR}.${ROSSNBOSS_REST_PRODUCT_VERSION_MINOR}.${ROSSNBOSS_REST_PRODUCT_VERSION_PATCH}-${ROSSNBOSS_REST_PRODUCT_VERSION_REVISION}" )

