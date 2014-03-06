#include <Wt/WString>
#include "exception.hpp"

using namespace std;
using namespace Wt;
using namespace RossNBoss;

Exception::Exception(const std::string &message) :
    CoreLib::Exception(message),
    m_message(WString::fromUTF8(message))
{

}

Exception::Exception(const std::wstring &message) :
    CoreLib::Exception(WString(message).toUTF8()),
    m_message(message)
{

}

const wchar_t *Exception::What() const
{
    return m_message.c_str();
}

