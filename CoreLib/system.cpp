#include <cstdio>
#include <cstdlib>
#if defined ( __unix__ )
#include <fcntl.h>
#include <unistd.h>
#endif  // defined ( __unix__ )
#include "system.hpp"
#include "make_unique.hpp"

using namespace std;
using namespace CoreLib;

struct System::Impl
{
#if defined ( __unix__ )
    static constexpr mode_t LOCK_FILE_PERMISSION = 0666;
#endif  // defined ( __unix__ )
};

bool System::Exec(const string &cmd)
{
    int r = system(cmd.c_str());
    if (r == EXIT_SUCCESS)
        return true;
    else
        return false;
}

bool System::Exec(const string &cmd, string &out_output)
{
    out_output.clear();

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return false;

    char buffer[256];
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != NULL) {
            out_output += buffer;
        }
    }

    pclose(pipe);
    return true;
}

#if defined ( __unix__ )

bool System::GetLock(const std::string &name, int &out_handle)
{
    struct flock fl;

    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 1;

    if ((out_handle = open(name.c_str(), O_WRONLY | O_CREAT, Impl::LOCK_FILE_PERMISSION)) == -1)
        return false;

    if (fcntl(out_handle, F_SETLK, &fl) == -1)
        return false;

    return true;
}

void System::ReleaseLock(int &handle)
{
    close(handle);
}

#elif defined ( _WIN32 )

bool System::GetLock(const std::string &name, HANDLE &out_handle)
{
    try {
        out_handle = OpenMutexA(MUTEX_ALL_ACCESS, TRUE, name.c_str());
        if (!out_handle) {
            out_handle = CreateMutexA(NULL, TRUE, name.c_str());
            return true;
        }
    } catch (...) {
    }

    return false;
}

void System::ReleaseLock(HANDLE &handle)
{
    ReleaseMutex(handle);
    CloseHandle(handle);
}

#endif  // defined ( __unix__ )

