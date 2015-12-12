IF ( WIN32 AND MINGW )
    SET ( MINGW_CPP_RUNTIME_FILES
        libgcc_s_dw2-1.dll
        libstdc++-6.dll
        libwinpthread-1.dll
    )
ENDIF ( WIN32 AND MINGW )


