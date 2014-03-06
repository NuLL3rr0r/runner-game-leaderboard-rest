#include <sstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <Wt/Http/Response>
#include <CoreLib/make_unique.hpp>
#include "apiresource.hpp"

using namespace std;
using namespace boost;
using namespace Wt;
using namespace CoreLib;
using namespace RossNBoss;

APIResource::APIResource(WObject *parent) :
    WResource(parent)
{

}

APIResource::~APIResource()
{

}

std::wstring APIResource::GetHTTPStatus(const CoreLib::HTTPStatus::HTTPStatusCode &code) const
{
    return HTTPStatus::GetHTTPResponse(code);
}

std::wstring APIResource::GetHTTPStatusJSON(const CoreLib::HTTPStatus::HTTPStatusCode &code) const
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"status", HTTPStatus::GetHTTPResponse(code));

    boost::property_tree::write_json(stream, tree);

    return stream.str();
}

std::wstring APIResource::GetHTTPStatusXML(const CoreLib::HTTPStatus::HTTPStatusCode &code) const
{
    std::wstringstream stream;
    boost::property_tree::wptree tree;

    tree.put(L"status", HTTPStatus::GetHTTPResponse(code));

    boost::property_tree::write_xml(stream, tree);

    return stream.str();
}

void APIResource::Print(Wt::Http::Response &response, const std::wstring &text)
{
    response.addHeader("Content-type", "text/plain; charset=utf-8");
    response.out() << text;
}

void APIResource::PrintJSON(Wt::Http::Response &response, const std::wstring &json)
{
    response.addHeader("Content-type", "application/json; charset=utf-8");
    response.out() << json;
}

void APIResource::PrintXML(Wt::Http::Response &response, const std::wstring &xml)
{
    response.addHeader("Content-type", "application/xml; charset=utf-8");
    response.out() << xml;
}

