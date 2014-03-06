#ifndef CORELIB_LOG_HPP
#define CORELIB_LOG_HPP


#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>

namespace CoreLib {
    class Log;
}

class CoreLib::Log
{
public:
    enum class EType : unsigned char {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

private:
    struct Impl;
    static std::unique_ptr<Impl> s_pimpl;

private:
    std::ostringstream m_buffer;
    bool m_hasEntries;

public:
    static void Initialize(std::ostream &out_outputStream);
    static void Initialize(const std::string &outputDirectoryPath,
                           const std::string &outputFilePrefix);
    static void Initialize(std::ostream &out_outputStream,
                           const std::string &outputDirectoryPath,
                           const std::string &outputFilePrefix);

public:
    Log(EType type, const std::string &file, const std::string &func, int line, ...);
    ~Log();

public:
    template<typename T>
    Log &operator,(const T &arg)
    {
        if (m_hasEntries)
            m_buffer << "\n";
        m_buffer << "  - " << arg;
        m_hasEntries = true;
        return *this;
    }
};


#define LOG_TRACE(...)  \
    (CoreLib::Log(CoreLib::Log::EType::Trace, __FILE__, __FUNCTION__, __LINE__)), __VA_ARGS__;

#define LOG_DEBUG(...)  \
    (CoreLib::Log(CoreLib::Log::EType::Debug, __FILE__, __FUNCTION__, __LINE__)), __VA_ARGS__;

#define LOG_INFO(...)  \
    (CoreLib::Log(CoreLib::Log::EType::Info, __FILE__, __FUNCTION__, __LINE__)), __VA_ARGS__;

#define LOG_WARNING(...)  \
    (CoreLib::Log(CoreLib::Log::EType::Warning, __FILE__, __FUNCTION__, __LINE__)), __VA_ARGS__;

#define LOG_ERROR(...)  \
    (CoreLib::Log(CoreLib::Log::EType::Error, __FILE__, __FUNCTION__, __LINE__)), __VA_ARGS__;

#define LOG_FATAL(...)  \
    (CoreLib::Log(CoreLib::Log::EType::Fatal, __FILE__, __FUNCTION__, __LINE__)), __VA_ARGS__;


#endif /* CORELIB_LOG_HPP */

