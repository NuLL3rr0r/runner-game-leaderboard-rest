#include <csignal>
#include <cstdlib>
#if defined ( _WIN32 )
#include <windows.h>
#else
#include <unistd.h>
#endif  // defined ( _WIN32 )
#include <boost/algorithm/string.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>
#include <Wt/WServer>
#include <CoreLib/corelib.hpp>
#include <CoreLib/exception.hpp>
#include <CoreLib/log.hpp>
#include <CoreLib/make_unique.hpp>
#include <CoreLib/system.hpp>
#include "dbtables.hpp"
#include "publicapiresource.hpp"
#include "rt.hpp"
#include "versioninfo.hpp"

void Terminate [[noreturn]] (int signo);

int main(int argc, char **argv)
{
    try {
        /// Gracefully handling SIGTERM
        void (*prev_fn)(int);
        prev_fn = signal(SIGTERM, Terminate);
        if (prev_fn == SIG_IGN)
            signal(SIGTERM, SIG_IGN);


        /// Extract the executable path and name
        boost::filesystem::path path(boost::filesystem::initial_path<boost::filesystem::path>());
        if (argc > 0 && argv[0] != NULL)
            path = boost::filesystem::system_complete(boost::filesystem::path(argv[0]));
        std::string appId(path.filename().string());
        std::string appPath(boost::algorithm::replace_last_copy(path.string(), appId, ""));
        RossNBoss::RT::Storage()->AppPath = appPath;


        /// Force changing the current path to executable path
        boost::filesystem::current_path(appPath);


        /// Initializing CoreLib
        CoreLib::CoreLibInitialize(argc, argv);


        /// Initializing log system
        CoreLib::Log::Initialize(std::cout,
                                 (boost::filesystem::path(appPath)
                                  / boost::filesystem::path("..")
                                  / boost::filesystem::path("log")).string(),
                                 "RossNBossServer");


        /// Acquiring process lock
        std::string lockId;
#if defined ( __unix__ )
        int lock;
        lockId = (boost::filesystem::path(appPath)
                  / boost::filesystem::path("..")
                  / boost::filesystem::path("tmp")
                  / (appId + ".lock")).string();
#elif defined ( _WIN32 )
        HANDLE lock;
        lockId = appId;
#endif  // defined ( __unix__ )


        /// Logging application info
        LOG_INFO("Version Information", "", "BUILD_COMPILER             " VERSION_INFO_BUILD_COMPILER, "BUILD_DATE                 " VERSION_INFO_BUILD_DATE, "BUILD_HOST                 " VERSION_INFO_BUILD_HOST, "BUILD_PROCESSOR            " VERSION_INFO_BUILD_PROCESSOR, "BUILD_SYSTEM               " VERSION_INFO_BUILD_SYSTEM, "PRODUCT_COMPANY_NAME       " VERSION_INFO_PRODUCT_COMPANY_NAME, "PRODUCT_COPYRIGHT          " VERSION_INFO_PRODUCT_COPYRIGHT, "PRODUCT_INTERNAL_NAME      " VERSION_INFO_PRODUCT_INTERNAL_NAME, "PRODUCT_NAME               " VERSION_INFO_PRODUCT_NAME, "PRODUCT_VERSION            " VERSION_INFO_PRODUCT_VERSION, "PRODUCT_DESCRIPTION        " VERSION_INFO_PRODUCT_DESCRIPTION);


        if(!CoreLib::System::GetLock(lockId, lock)) {
            LOG_WARNING("Process is already running!");
        } else {
            LOG_INFO("Got the process lock!");

            // Initialize database structure
            RossNBoss::RT::DBTables()->InitTables();
        }

        /// Starting RESTful service server, otherwise going down
        LOG_INFO("Starting RESTful service server...");
        Wt::WServer server(argv[0]);
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        RossNBoss::PublicAPIResource publicAPIResource;
        server.addResource(&publicAPIResource, "/RossNBoss");
        if (server.start()) {
            Wt::WServer::waitForShutdown();
            server.stop();
        }
    }

    catch (CoreLib::Exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (Wt::WServer::Exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (boost::exception &ex) {
        LOG_ERROR(boost::diagnostic_information(ex));
    }

    catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }

    catch (...) {
        LOG_ERROR("Unknown error!");
    }

    return EXIT_SUCCESS;
}

void Terminate(int signo)
{
    LOG_WARNING((boost::format("Terminating by signal %1%...") % signo).str());
    exit(1);
}

