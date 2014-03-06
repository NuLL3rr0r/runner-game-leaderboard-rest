#include <fstream>
#include <curlpp/internal/SList.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include "http.hpp"

#define     UNKNOWN_ERROR           "REST::HTTP unknown error!"
#define     OPEN_FILE_ERROR         "REST::HTTP could not open file!"

using namespace std;
using namespace curlpp;
using namespace CoreLib;

bool HTTP::DownloadFile(const std::string &remoteAddr, const std::string &localPath)
{
    string err;
    return DownloadFile(remoteAddr.c_str(), localPath, err);
}

bool HTTP::DownloadFile(const std::string &remoteAddr, const std::string &localPath,
                        std::string &out_error)
{
    try {
        out_error.clear();

        // It is required to do cleanup of used resources
        Cleanup cleanup;
        (void)cleanup;

        std::list<std::string> headers;
        headers.push_back("ACCEPT: text/html, */*; q=0.01");
        headers.push_back("ACCEPT_CHARSET: utf-8;q=0.7,*;q=0.7");
        headers.push_back("ACCEPT_ENCODING: gzip, deflate");
        headers.push_back("ACCEPT_LANGUAGE: en-US,en;q=0.5");
        headers.push_back("CONNECTION: keep-alive");
        headers.push_back("KEEP_ALIVE: 300");

        internal::SList slist(headers);

        std::string userAgent = "Mozilla/5.0 (Windows; FreeBSD amd64; rv:26.0) Gecko/20100101 Firefox/26.0";

        curlpp::Easy request;
        request.setOpt<options::HttpHeader>(slist);
        request.setOpt<options::UserAgent>(userAgent.c_str());

        // If an empty string, "", is set, a header containing all supported encoding types is sent.
        // Supported encodings are "identity", "deflate", and "gzip".
        request.setOpt<options::Encoding>("");

        request.setOpt<options::Url>(remoteAddr);
        std::ofstream ofs(localPath, std::ios::binary);

        if (!ofs.is_open()) {
            out_error.assign(OPEN_FILE_ERROR);
            return false;
        }

        options::WriteStream ws(&ofs);
        request.setOpt(ws);
        request.perform();

        ofs.flush();
        ofs.close();

        return true;
    }

    catch (const curlpp::RuntimeError &ex) {
        out_error.assign(ex.what());
    }

    catch (const curlpp::LogicError &ex) {
        out_error.assign(ex.what());
    }

    catch (const std::runtime_error &ex) {
        out_error.assign(ex.what());
    }

    catch (...) {
        out_error.assign(UNKNOWN_ERROR);
    }

    return false;
}

