#include <boost/filesystem/path.hpp>
#include <CoreLib/crypto.hpp>
#include <CoreLib/db.hpp>
#include <CoreLib/make_unique.hpp>
#include <CoreLib/log.hpp>
#include "rt.hpp"
#include "dbtables.hpp"

using namespace std;
using namespace RossNBoss;

struct RT::Impl
{
    typedef std::unique_ptr<StorageStruct> Storage_ptr;
    typedef std::unique_ptr<CoreLib::Crypto> Crypto_ptr;
    typedef std::unique_ptr<CoreLib::DB> DB_ptr;
    typedef std::unique_ptr<RossNBoss::DBTables> DBTables_ptr;

    std::mutex StorageMutex;
    Storage_ptr StorageInstance;

    std::mutex DBMutex;
    DB_ptr DBInstance;

    std::mutex DBTablesMutex;
    DBTables_ptr DBTablesInstance;

    std::mutex TokenClientMutex;
    Crypto_ptr TokenClientInstance;

    std::mutex TokenServerMutex;
    Crypto_ptr TokenServerInstance;

    Impl();
    ~Impl();
};

std::unique_ptr<RT::Impl> RT::s_pimpl = std::make_unique<RT::Impl>();

RT::StorageStruct *RT::Storage()
{
    lock_guard<mutex> lock(s_pimpl->StorageMutex);
    (void)lock;

    if (s_pimpl->StorageInstance == nullptr) {
        s_pimpl->StorageInstance = std::make_unique<RT::StorageStruct>();
    }

    return s_pimpl->StorageInstance.get();
}

CoreLib::DB *RT::DB()
{
    lock_guard<mutex> lock(s_pimpl->DBMutex);
    (void)lock;

    if (s_pimpl->DBInstance == nullptr) {
        static const std::string DB_FILE((boost::filesystem::path(Storage()->AppPath)
                                          / boost::filesystem::path("..")
                                          / boost::filesystem::path("db")
                                          / boost::filesystem::path(SCORES_DB_FILE_NAME)).string());
        CoreLib::DB::Vacuum(DB_FILE);
#ifdef CORELIB_STATIC
        DB::LoadSQLite3Driver();
#endif  // CORELIB_STATIC
        s_pimpl->DBInstance = std::make_unique<CoreLib::DB>(DB_FILE);
    }

    return s_pimpl->DBInstance.get();
}

RossNBoss::DBTables *RT::DBTables()
{
    lock_guard<mutex> lock(s_pimpl->DBTablesMutex);
    (void)lock;

    if (s_pimpl->DBTablesInstance == nullptr) {
        s_pimpl->DBTablesInstance = std::make_unique<RossNBoss::DBTables>();
    }

    return s_pimpl->DBTablesInstance.get();
}

CoreLib::Crypto *RT::TokenClient()
{
    lock_guard<mutex> lock(s_pimpl->TokenClientMutex);
    (void)lock;

    if (s_pimpl->TokenClientInstance == nullptr) {
        // Use this nice HEX/ASCII converter and your editor's replace dialog,
        // to create your own Key and IV.
        // http://www.dolcevie.com/js/converter.html

        // `7mWgH7kKy1\.%#^
        static constexpr CoreLib::Crypto::Byte_t KEY[] = {
            0x60, 0x37, 0x6d, 0x57, 0x67, 0x48, 0x37, 0x6b, 0x4b, 0x79, 0x31, 0x5c, 0x2e, 0x25, 0x23, 0x5e
        };

        // `imDbC.!Mt1/.?@3
        static constexpr CoreLib::Crypto::Byte_t IV[] = {
            0x60, 0x69, 0x6d, 0x44, 0x62, 0x43, 0x2e, 0x21, 0x4d, 0x74, 0x31, 0x2f, 0x2e, 0x3f, 0x40, 0x33
        };

        s_pimpl->TokenClientInstance = std::make_unique<CoreLib::Crypto>(KEY, sizeof(KEY), IV, sizeof(IV));
    }

    return s_pimpl->TokenClientInstance.get();
}

CoreLib::Crypto *RT::TokenServer()
{
    lock_guard<mutex> lock(s_pimpl->TokenServerMutex);
    (void)lock;

    if (s_pimpl->TokenServerInstance == nullptr) {
        // Use this nice HEX/ASCII converter and your editor's replace dialog,
        // to create your own Key and IV.
        // http://www.dolcevie.com/js/converter.html

        // `#3BS73kKyp/.?2e
        static constexpr CoreLib::Crypto::Byte_t KEY[] = {
            0x60, 0x23, 0x33, 0x42, 0x53, 0x37, 0x33, 0x6b, 0x4b, 0x79, 0x70, 0x2f, 0x2e, 0x3f, 0x32, 0x65
        };

        // V1MzS7wLoP3/&*3d
        static constexpr CoreLib::Crypto::Byte_t IV[] = {
            0x56, 0x31, 0x4d, 0x7a, 0x53, 0x37, 0x77, 0x4c, 0x6f, 0x50, 0x33, 0x2f, 0x26, 0x2a, 0x33, 0x64
        };

        s_pimpl->TokenServerInstance = std::make_unique<CoreLib::Crypto>(KEY, sizeof(KEY), IV, sizeof(IV));
    }

    return s_pimpl->TokenServerInstance.get();
}

RT::Impl::Impl()
{

}

RT::Impl::~Impl()
{
    StorageInstance.reset();
    DBInstance.reset();
    TokenClientInstance.reset();
    TokenServerInstance.reset();
}

