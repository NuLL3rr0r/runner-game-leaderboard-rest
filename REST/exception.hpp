#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP


#include <string>
#include <CoreLib/exception.hpp>

namespace RossNBoss {
    class Exception;
}

class RossNBoss::Exception : public CoreLib::Exception
{
protected:
    std::wstring m_message;

public:
    explicit Exception(const std::string &message);
    explicit Exception(const std::wstring &message);

public:
    virtual const wchar_t *What() const;
};


#endif /* EXCEPTION_HPP */

