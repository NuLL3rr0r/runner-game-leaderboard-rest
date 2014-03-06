#ifndef XML_EXCEPTION_HPP
#define XML_EXCEPTION_HPP


#include <string>
#include "exception.hpp"

namespace RossNBoss {
    class XMLException;
}

class RossNBoss::XMLException : public RossNBoss::Exception
{
public:
    explicit XMLException(const std::string &message);
    explicit XMLException(const std::wstring &message);

public:
    virtual const wchar_t *What() const;
};


#endif /* XML_EXCEPTION_HPP */

