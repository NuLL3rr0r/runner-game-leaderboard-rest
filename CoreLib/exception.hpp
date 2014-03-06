#ifndef CORELIB_EXCEPTION_HPP
#define CORELIB_EXCEPTION_HPP


#include <stdexcept>
#include <string>

namespace CoreLib {
    class Exception;
}

class CoreLib::Exception : public std::runtime_error
{
public:
    explicit Exception(const std::string &message);
};


#endif /* CORELIB_EXCEPTION_HPP */

