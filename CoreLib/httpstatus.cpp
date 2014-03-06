#include <unordered_map>
#include <boost/format.hpp>
#include "httpstatus.hpp"
#include "make_unique.hpp"

using namespace  std;
using namespace  boost;
using namespace  CoreLib;

#define     HTTP_VERSION        L"HTTP/1.1"
#define     HTTP_100_DEF        L"100 Continue"
#define     HTTP_101_DEF        L"101 Switching Protocols"
#define     HTTP_200_DEF        L"200 OK"
#define     HTTP_201_DEF        L"201 Created"
#define     HTTP_202_DEF        L"202 Accepted"
#define     HTTP_203_DEF        L"203 Non-Authoritative Information"
#define     HTTP_204_DEF        L"204 No Content"
#define     HTTP_205_DEF        L"205 Reset Content"
#define     HTTP_206_DEF        L"206 Partial Content"
#define     HTTP_300_DEF        L"300 Multiple Choices"
#define     HTTP_301_DEF        L"301 Moved Permanently"
#define     HTTP_302_DEF        L"302 Found"
#define     HTTP_303_DEF        L"303 See Other"
#define     HTTP_304_DEF        L"304 Not Modified"
#define     HTTP_305_DEF        L"305 Use Proxy"
#define     HTTP_307_DEF        L"307 Temporary Redirect"
#define     HTTP_400_DEF        L"400 Bad Request"
#define     HTTP_401_DEF        L"401 Unauthorized"
#define     HTTP_402_DEF        L"402 Payment Required"
#define     HTTP_403_DEF        L"403 Forbidden"
#define     HTTP_404_DEF        L"404 Not Found"
#define     HTTP_405_DEF        L"405 Method Not Allowed"
#define     HTTP_406_DEF        L"406 Not Acceptable"
#define     HTTP_407_DEF        L"407 Proxy Authentication Required"
#define     HTTP_408_DEF        L"408 Request Timeout"
#define     HTTP_409_DEF        L"409 Conflict"
#define     HTTP_410_DEF        L"410 Gone"
#define     HTTP_411_DEF        L"411 Length Required"
#define     HTTP_412_DEF        L"412 Precondition Failed"
#define     HTTP_413_DEF        L"413 Request Entity Too Large"
#define     HTTP_414_DEF        L"414 Request-URI Too Long"
#define     HTTP_415_DEF        L"415 Unsupported Media Type"
#define     HTTP_416_DEF        L"416 Requested Range Not Satisfiable"
#define     HTTP_417_DEF        L"417 Expectation Failed"
#define     HTTP_500_DEF        L"500 Internal Server Error"
#define     HTTP_501_DEF        L"501 Not Implemented"
#define     HTTP_502_DEF        L"502 Bad Gateway"
#define     HTTP_503_DEF        L"503 Service Unavailable"
#define     HTTP_504_DEF        L"504 Gateway Timeout"
#define     HTTP_505_DEF        L"505 HTTP Version Not Supported"

struct HTTPStatus::Impl
{
    template <typename _T>
    struct Hasher
    {
        std::size_t operator()(const _T &t) const
        {
            return std::hash<unsigned char>()(static_cast<unsigned char>(t));
        }
    };

    typedef unordered_map<HTTPStatusCode, std::wstring, Hasher<HTTPStatusCode>> StatusCodes_t;
    StatusCodes_t StatusCodes;

    Impl();
    ~Impl();
};

std::unique_ptr<HTTPStatus::Impl> HTTPStatus::s_pimpl = std::make_unique<HTTPStatus::Impl>();

std::wstring HTTPStatus::GetHTTPResponse(const HTTPStatusCode &code)
{
    return (boost::wformat(L"%1% %2%") % HTTP_VERSION % s_pimpl->StatusCodes[code]).str();
}

HTTPStatus::Impl::Impl()
{
    StatusCodes[HTTPStatusCode::HTTP_100] = HTTP_100_DEF;
    StatusCodes[HTTPStatusCode::HTTP_101] = HTTP_101_DEF;
    StatusCodes[HTTPStatusCode::HTTP_200] = HTTP_200_DEF;
    StatusCodes[HTTPStatusCode::HTTP_201] = HTTP_201_DEF;
    StatusCodes[HTTPStatusCode::HTTP_202] = HTTP_202_DEF;
    StatusCodes[HTTPStatusCode::HTTP_203] = HTTP_203_DEF;
    StatusCodes[HTTPStatusCode::HTTP_204] = HTTP_204_DEF;
    StatusCodes[HTTPStatusCode::HTTP_205] = HTTP_205_DEF;
    StatusCodes[HTTPStatusCode::HTTP_206] = HTTP_206_DEF;
    StatusCodes[HTTPStatusCode::HTTP_300] = HTTP_300_DEF;
    StatusCodes[HTTPStatusCode::HTTP_301] = HTTP_301_DEF;
    StatusCodes[HTTPStatusCode::HTTP_302] = HTTP_302_DEF;
    StatusCodes[HTTPStatusCode::HTTP_303] = HTTP_303_DEF;
    StatusCodes[HTTPStatusCode::HTTP_304] = HTTP_304_DEF;
    StatusCodes[HTTPStatusCode::HTTP_305] = HTTP_305_DEF;
    StatusCodes[HTTPStatusCode::HTTP_307] = HTTP_307_DEF;
    StatusCodes[HTTPStatusCode::HTTP_400] = HTTP_400_DEF;
    StatusCodes[HTTPStatusCode::HTTP_401] = HTTP_401_DEF;
    StatusCodes[HTTPStatusCode::HTTP_402] = HTTP_402_DEF;
    StatusCodes[HTTPStatusCode::HTTP_403] = HTTP_403_DEF;
    StatusCodes[HTTPStatusCode::HTTP_404] = HTTP_404_DEF;
    StatusCodes[HTTPStatusCode::HTTP_405] = HTTP_405_DEF;
    StatusCodes[HTTPStatusCode::HTTP_406] = HTTP_406_DEF;
    StatusCodes[HTTPStatusCode::HTTP_407] = HTTP_407_DEF;
    StatusCodes[HTTPStatusCode::HTTP_408] = HTTP_408_DEF;
    StatusCodes[HTTPStatusCode::HTTP_409] = HTTP_409_DEF;
    StatusCodes[HTTPStatusCode::HTTP_410] = HTTP_410_DEF;
    StatusCodes[HTTPStatusCode::HTTP_411] = HTTP_411_DEF;
    StatusCodes[HTTPStatusCode::HTTP_412] = HTTP_412_DEF;
    StatusCodes[HTTPStatusCode::HTTP_413] = HTTP_413_DEF;
    StatusCodes[HTTPStatusCode::HTTP_414] = HTTP_414_DEF;
    StatusCodes[HTTPStatusCode::HTTP_415] = HTTP_415_DEF;
    StatusCodes[HTTPStatusCode::HTTP_416] = HTTP_416_DEF;
    StatusCodes[HTTPStatusCode::HTTP_417] = HTTP_417_DEF;
    StatusCodes[HTTPStatusCode::HTTP_500] = HTTP_500_DEF;
    StatusCodes[HTTPStatusCode::HTTP_501] = HTTP_501_DEF;
    StatusCodes[HTTPStatusCode::HTTP_502] = HTTP_502_DEF;
    StatusCodes[HTTPStatusCode::HTTP_503] = HTTP_503_DEF;
    StatusCodes[HTTPStatusCode::HTTP_504] = HTTP_504_DEF;
    StatusCodes[HTTPStatusCode::HTTP_505] = HTTP_505_DEF;
}

HTTPStatus::Impl::~Impl()
{
}

