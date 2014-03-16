#include <chrono>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/Utils>
#include <Wt/WString>
#include <CoreLib/crypto.hpp>
#include <CoreLib/db.hpp>
#include <CoreLib/log.hpp>
#include <CoreLib/make_unique.hpp>
#include "publicapiresource.hpp"
#include "dbtables.hpp"
#include "jsonexception.hpp"
#include "rt.hpp"
#include "servicecontract.hpp"
#include "xmlexception.hpp"

#define     MAX_TOKEN_MILLISECONDS_DIFFERENCE        240000

#define     INVALID_TOKEN_ERROR                      L"INVALID_TOKEN"

#define     PostPlayerScoreJSON_URI_TEMPLATE         L"RossNBoss/PostPlayerScore/JSON/{NAME}/{SCORE}/{TOKEN}"
#define     PostPlayerScoreXML_URI_TEMPLATE          L"RossNBoss/PostPlayerScore/XML/{NAME}/{SCORE}/{TOKEN}"
#define     TopScoresJSON_URI_TEMPLATE               L"RossNBoss/TopScores/JSON/{LIMIT}/{TOKEN}"
#define     TopScoresXML_URI_TEMPLATE                L"RossNBoss/TopScores/XML/{LIMIT}/{TOKEN}"
#define     TokenJSON_URI_TEMPLATE                   L"RossNBoss/Token/JSON"
#define     TokenXML_URI_TEMPLATE                    L"RossNBoss/Token/XML"

using namespace std;
using namespace boost;
using namespace Wt;
using namespace RossNBoss;

struct PublicAPIResource::Impl
{
    typedef vector<vector<std::string> > Table_t;
    typedef vector<std::string> Row_t;

    enum class OutputType : unsigned char {
        JSON,
        XML
    };

    std::unique_ptr<RossNBoss::ServiceContract> ServiceContractPtr;

    bool IsValidToken(const std::wstring &encryptedToken);

    void PostPlayerScore(const std::string &name, const std::string &score,
                         boost::property_tree::wptree &out_tree);
    void GetTopScoresTree(const OutputType &outputType,
                          const Row_t &titles, const Table_t &data,
                          boost::property_tree::wptree &out_tree);
    void GetTopScores(const OutputType &outputType, size_t limit,
                      boost::property_tree::wptree &out_tree);
    void GetToken(boost::property_tree::wptree &out_tree);

    void PostPlayerScoreJSON(const std::wstring &name, const std::wstring &score,
                             std::wstring &out_response);
    void PostPlayerScoreXML(const std::wstring &name, const std::wstring &score,
                            std::wstring &out_response);
    void TopScoresJSON(size_t limit, std::wstring &out_response);
    void TopScoresXML(size_t limit, std::wstring &out_response);
    void TokenJSON(std::wstring &out_response);
    void TokenXML(std::wstring &out_response);

    Impl();
    ~Impl();
};

PublicAPIResource::PublicAPIResource(WObject *parent) :
    APIResource(parent),
    m_pimpl(std::make_unique<PublicAPIResource::Impl>())
{
    m_pimpl->ServiceContractPtr = std::make_unique<RossNBoss::ServiceContract>();
    m_pimpl->ServiceContractPtr->Register(PostPlayerScoreJSON_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(PostPlayerScoreXML_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(TopScoresJSON_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(TopScoresXML_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(TokenJSON_URI_TEMPLATE);
    m_pimpl->ServiceContractPtr->Register(TokenXML_URI_TEMPLATE);
}

PublicAPIResource::~PublicAPIResource()
{
    beingDeleted();
}

void PublicAPIResource::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
    try {
        WString uri((boost::format("%1%%2%")
                     % request.path().substr(request.path().find_last_of("/") + 1)
                     % request.pathInfo()
                     ).str());
        wstring uriTemplate;
        vector<wstring> args;

        if (m_pimpl->ServiceContractPtr->Resolve(uri.value(), uriTemplate, args)) {
            wstring outResponse;

            /// Validating the token
            if (boost::algorithm::contains(uriTemplate, L"/{TOKEN}")) {
                if (!m_pimpl->IsValidToken(args[args.size() - 1])) {
                    if (boost::algorithm::contains(uriTemplate, L"/JSON")) {
                        throw RossNBoss::JSONException(INVALID_TOKEN_ERROR);
                    } else if (boost::algorithm::contains(uriTemplate, L"/XML")) {
                        throw RossNBoss::XMLException(INVALID_TOKEN_ERROR);
                    } else {
                        throw RossNBoss::Exception(INVALID_TOKEN_ERROR);
                    }
                }
            }

            if (uriTemplate == PostPlayerScoreJSON_URI_TEMPLATE) {

                m_pimpl->PostPlayerScoreJSON(args[0], args[1], outResponse);
                PrintJSON(response, outResponse);

            } else if (uriTemplate == PostPlayerScoreXML_URI_TEMPLATE) {

                m_pimpl->PostPlayerScoreXML(args[0], args[1], outResponse);
                PrintXML(response, outResponse);

            } else if (uriTemplate == TopScoresJSON_URI_TEMPLATE) {

                m_pimpl->TopScoresJSON(boost::lexical_cast<size_t>(args[0]), outResponse);
                PrintJSON(response, outResponse);

            } else if (uriTemplate == TopScoresXML_URI_TEMPLATE) {

                m_pimpl->TopScoresXML(boost::lexical_cast<size_t>(args[0]), outResponse);
                PrintXML(response, outResponse);

            } else if (uriTemplate == TokenJSON_URI_TEMPLATE) {

                m_pimpl->TokenJSON(outResponse);
                PrintJSON(response, outResponse);

            } else if (uriTemplate == TokenXML_URI_TEMPLATE) {

                m_pimpl->TokenXML(outResponse);
                PrintXML(response, outResponse);

            }
        } else {
            if (boost::algorithm::contains(uri.value(), L"/JSON")) {
                PrintJSON(response, GetHTTPStatusJSON(CoreLib::HTTPStatus::HTTPStatusCode::HTTP_400));
            } else if (boost::algorithm::contains(uri.value(), L"/XML")) {
                PrintXML(response, GetHTTPStatusXML(CoreLib::HTTPStatus::HTTPStatusCode::HTTP_400));
            } else {
                Print(response, GetHTTPStatus(CoreLib::HTTPStatus::HTTPStatusCode::HTTP_400));
            }
        }
    }

    catch (const RossNBoss::JSONException &ex) {
        PrintJSON(response, ex.What());
    }

    catch (const RossNBoss::XMLException &ex) {
        PrintXML(response, ex.What());
    }

    catch (const RossNBoss::Exception &ex) {
        Print(response, ex.What());
    }

    catch (...) {
        Print(response, GetHTTPStatus(CoreLib::HTTPStatus::HTTPStatusCode::HTTP_500));
    }
}

PublicAPIResource::Impl::Impl()
{

}

PublicAPIResource::Impl::~Impl()
{

}

bool PublicAPIResource::Impl::IsValidToken(const std::wstring &encryptedToken)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    double token;

    try {
        std::string decryptedToken;
        RT::TokenClient()->Decrypt(WString(encryptedToken).toUTF8(), decryptedToken);
        token = boost::lexical_cast<double>(decryptedToken);
    } catch (...) {
        return false;
    }

    if (std::abs((std::chrono::system_clock::to_time_t(now) * 1000.0) - token)
            > MAX_TOKEN_MILLISECONDS_DIFFERENCE) {
        return false;
    }

    return true;
}

void PublicAPIResource::Impl::PostPlayerScore(const std::string &name, const std::string &score,
                                              boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    cppdb::transaction guard(RT::DB()->SQL());

    RT::DB()->Insert(RT::DBTables()->Table("SCORES"),
                     "name, score", 2,
                     name.c_str(),
                     score.c_str());

    guard.commit();

    out_tree.put(L"status", CoreLib::HTTPStatus::GetHTTPResponse(
                     CoreLib::HTTPStatus::HTTPStatusCode::HTTP_200));
}

void PublicAPIResource::Impl::GetTopScoresTree(const OutputType &outputType,
                                               const Row_t &titles, const Table_t &data,
                                               boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    out_tree.put(L"RossNBoss.version.major", "1");
    out_tree.put(L"RossNBoss.version.minor", "0");
    out_tree.put(L"RossNBoss.version.compat.major", "1");
    out_tree.put(L"RossNBoss.version.compat.minor", "0");

    boost::property_tree::wptree titlesTree;
    for (Row_t::const_iterator it = titles.begin(); it != titles.end(); ++it) {
        boost::property_tree::wptree nameTree;
        nameTree.put(L"", WString(*it).value());

        switch (outputType) {
        case OutputType::JSON:
            titlesTree.push_back(std::make_pair(L"", nameTree));
            break;
        case OutputType::XML:
            titlesTree.add_child(L"n", nameTree);
            break;
        }
    }
    out_tree.add_child(L"RossNBoss.titles", titlesTree);

    boost::property_tree::wptree dataTree;
    for (Table_t::const_iterator it = data.begin(); it != data.end(); ++it) {
        boost::property_tree::wptree rowTree;
        for (Row_t::const_iterator rowIt = (*it).begin(); rowIt != (*it).end(); ++rowIt) {
            boost::property_tree::wptree colTree;
            colTree.put(L"", WString(*rowIt).value());

            switch (outputType) {
            case OutputType::JSON:
                rowTree.push_back(std::make_pair(L"", colTree));
                break;
            case OutputType::XML:
                rowTree.add_child(L"c", colTree);
                break;
            }
        }

        switch (outputType) {
        case OutputType::JSON:
            dataTree.push_back(std::make_pair(L"", rowTree));
            break;
        case OutputType::XML:
            dataTree.add_child(L"r", rowTree);
            break;
        }
    }
    out_tree.add_child(L"RossNBoss.data", dataTree);
}

void PublicAPIResource::Impl::GetTopScores(const OutputType &outputType, size_t limit,
                                           boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    Row_t titles;
    Table_t data;

    titles.push_back("name");
    titles.push_back("score");

    cppdb::transaction guard(RT::DB()->SQL());

    cppdb::result r = RT::DB()->SQL()
            << (boost::format("SELECT name, score"
                              " FROM %1%"
                              " ORDER BY score DESC"
                              " LIMIT %2%;")
                % RT::DBTables()->Table("SCORES")
                % limit).str();

    string name;
    string score;

    while(r.next()) {
        r >> name >> score;
        Row_t row { name, score };
        data.push_back(row);
    }

    guard.rollback();

    GetTopScoresTree(outputType, titles, data, out_tree);
}

void PublicAPIResource::Impl::GetToken(boost::property_tree::wptree &out_tree)
{
    out_tree.clear();

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> millisecondsSinceEpoch =
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    std::string token;
    RT::TokenServer()->Encrypt(lexical_cast<std::string>(millisecondsSinceEpoch.count()), token);

    out_tree.put(L"token", WString(token).value());
}

void PublicAPIResource::Impl::PostPlayerScoreJSON(const std::wstring &name, const std::wstring &score,
                                                  std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    PostPlayerScore(WString(name).toUTF8(), WString(score).toUTF8(), tree);

    boost::property_tree::write_json(stream, tree);

    out_response.assign(stream.str());
}

void PublicAPIResource::Impl::PostPlayerScoreXML(const std::wstring &name, const std::wstring &score,
                                                 std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    PostPlayerScore(WString(name).toUTF8(), WString(score).toUTF8(), tree);

    boost::property_tree::write_xml(stream, tree);

    out_response.assign(stream.str());
}

void PublicAPIResource::Impl::TopScoresJSON(size_t limit, std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetTopScores(OutputType::JSON, limit, tree);

    boost::property_tree::write_json(stream, tree);

    out_response.assign(stream.str());
}

void PublicAPIResource::Impl::TopScoresXML(size_t limit, std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetTopScores(OutputType::XML, limit, tree);

    boost::property_tree::write_xml(stream, tree);

    out_response.assign(stream.str());
}

void PublicAPIResource::Impl::TokenJSON(std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetToken(tree);

    boost::property_tree::write_json(stream, tree);

    out_response.assign(stream.str());
}

void PublicAPIResource::Impl::TokenXML(std::wstring &out_response)
{
    out_response.clear();

    std::wstringstream stream;
    boost::property_tree::wptree tree;

    GetToken(tree);

    boost::property_tree::write_xml(stream, tree);

    out_response.assign(stream.str());
}


