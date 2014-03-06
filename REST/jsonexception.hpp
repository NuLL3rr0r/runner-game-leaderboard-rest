#ifndef JSON_EXCEPTION_HPP
#define JSON_EXCEPTION_HPP


#include <string>
#include "exception.hpp"

namespace RossNBoss {
    class JSONException;
}

class RossNBoss::JSONException : public RossNBoss::Exception
{
public:
    explicit JSONException(const std::string &message);
    explicit JSONException(const std::wstring &message);

public:
    virtual const wchar_t *What() const;
};


#endif /* JSON_EXCEPTION_HPP */

