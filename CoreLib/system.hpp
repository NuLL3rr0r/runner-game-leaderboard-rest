#ifndef CORELIB_SYSTEM_HPP
#define CORELIB_SYSTEM_HPP


#include <string>

#if defined ( __unix__ )
#include <sys/types.h>
#elif defined ( _WIN32 )
#include <windows.h>
#include <winbase.h>    // Synchapi.h on Windows 8 and Windows Server 2012
#endif  // defined ( __unix__ )

namespace CoreLib {
    class System;
}

class CoreLib::System
{
private:
    struct Impl;

public:
    static bool Exec(const std::string &cmd);
    static bool Exec(const std::string &cmd, std::string &out_output);

#if defined ( __unix__ )
    static bool GetLock(const std::string &name, int &out_handle);
    static void ReleaseLock(int &handle);
#elif defined ( _WIN32 )
    static bool GetLock(const std::string &name, HANDLE &out_handle);
    static void ReleaseLock(HANDLE &handle);
#endif  // defined ( __unix__ )
};


#endif /* CORELIB_SYSTEM_HPP */


