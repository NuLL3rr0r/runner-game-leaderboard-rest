#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <Wt/WString>
#include <CoreLib/make_unique.hpp>
#include "jsonexception.hpp"

using namespace std;
using namespace RossNBoss;
using namespace Wt;

JSONException::JSONException(const std::string &message) :
    Exception(message)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"error", WString(message).value());

    boost::property_tree::write_json(stream, tree);

    m_message = stream.str();
}

JSONException::JSONException(const std::wstring &message) :
    Exception(message)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"error", message);

    boost::property_tree::write_json(stream, tree);

    m_message = stream.str();
}

const wchar_t *JSONException::What() const
{
    return m_message.c_str();
}

