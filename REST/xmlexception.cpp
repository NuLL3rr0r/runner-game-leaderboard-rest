#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <Wt/WString>
#include <CoreLib/make_unique.hpp>
#include "xmlexception.hpp"

using namespace std;
using namespace RossNBoss;
using namespace Wt;

XMLException::XMLException(const std::string &message) :
    Exception(message)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"error", WString(message).value());

    boost::property_tree::write_xml(stream, tree);

    m_message = stream.str();
}

XMLException::XMLException(const std::wstring &message) :
    Exception(message)
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"error", message);

    boost::property_tree::write_xml(stream, tree);

    m_message = stream.str();
}

const wchar_t *XMLException::What() const
{
    return m_message.c_str();
}

