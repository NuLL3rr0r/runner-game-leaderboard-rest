#ifndef API_RESOURCE_HPP
#define API_RESOURCE_HPP


#include <Wt/WResource>

namespace Wt {
    namespace Http {
        class Request;
        class Response;
    }
}

#include <CoreLib/httpstatus.hpp>

namespace RossNBoss {
    class APIResource;
}

class RossNBoss::APIResource : public Wt::WResource
{
public:
    explicit APIResource(WObject *parent = NULL);
    virtual ~APIResource() = 0;

public:
    virtual void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response) = 0;

protected:
    std::wstring GetHTTPStatus(const CoreLib::HTTPStatus::HTTPStatusCode &code) const;
    std::wstring GetHTTPStatusJSON(const CoreLib::HTTPStatus::HTTPStatusCode &code) const;
    std::wstring GetHTTPStatusXML(const CoreLib::HTTPStatus::HTTPStatusCode &code) const;

    void Print(Wt::Http::Response &response, const std::wstring &text);
    void PrintJSON(Wt::Http::Response &response, const std::wstring &json);
    void PrintXML(Wt::Http::Response &response, const std::wstring &xml);
};


#endif /* API_RESOURCE_HPP */

