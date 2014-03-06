#ifndef CORELIB_HTTP_HPP
#define CORELIB_HTTP_HPP


#include <string>

namespace CoreLib {
    class HTTP;
}

class CoreLib::HTTP
{
public:
    static bool DownloadFile(const std::string &remoteAddr, const std::string &localPath);
    static bool DownloadFile(const std::string &remoteAddr, const std::string &localPath,
                             std::string &out_error);
};


#endif /* CORELIB_HTTP_HPP */

