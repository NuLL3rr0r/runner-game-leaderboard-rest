#include <cassert>
#include <cstdarg>
#include <sqlite3.h>
#include <cppdb/backend.h>
#include <cppdb/driver_manager.h>
#include "db.hpp"
#include "make_unique.hpp"

using namespace std;
using namespace cppdb;
using namespace CoreLib;

struct DB::Impl
{
#ifdef CORELIB_STATIC
    static bool DB::IsSQLite3DriverLoaded = false;
#endif  // CORELIB_STATIC

    cppdb::session SQL;

    Impl();
    ~Impl();
};

#ifdef CORELIB_STATIC
extern "C" {
cppdb::backend::connection *cppdb_sqlite3_get_connection(cppdb::connection_info const &);
}
#endif  // defined ( CORELIB_STATIC )

#ifdef CORELIB_STATIC
void DB::LoadSQLite3Driver()
{
    if (!Impl::IsSQLite3DriverLoaded) {
        m_isSQLite3DriverLoaded = true;
        cppdb::driver_manager::instance()
                .install_driver("sqlite3",
                                new cppdb::backend::static_driver(cppdb_sqlite3_get_connection));
    }
}
#endif  // defined ( CORELIB_STATIC )

bool DB::Vacuum(const std::string &dbFile)
{
    sqlite3 *db;

    int rc = sqlite3_open(dbFile.c_str(), &db);
    if (!rc) {
        sqlite3_exec(db, "VACUUM;", 0, 0, 0);
        return true;
    }

    return false;
}

DB::DB(const string &dbFile) :
    m_pimpl(std::make_unique<DB::Impl>())
{
    assert(sqlite3_enable_shared_cache(true) == SQLITE_OK);
    m_pimpl->SQL.open("sqlite3:db=" + dbFile);
}

DB::~DB()
{
    m_pimpl->SQL.close();
}

cppdb::session &DB::SQL()
{
    return m_pimpl->SQL;
}

void DB::CreateTable(const std::string &table, const std::string &fields)
{
    m_pimpl->SQL << "CREATE TABLE IF NOT EXISTS [" + table + "] ("
                    + fields
                    + ");"
                 << exec;
}

void DB::DropTable(const std::string &table)
{
    m_pimpl->SQL << "DROP TABLE IF EXISTS [" + table + "];"
                 << exec;
}

void DB::RenameTable(const std::string &table, const std::string &newTable)
{
    m_pimpl->SQL << "ALTER TABLE [" + table + "] RENAME TO [" +  newTable + "];"
                 << exec;
}

void DB::Insert(const std::string &table, const std::string &fields, const int count, ...)
{
    va_list args;
    va_start(args, count);

    string ph;
    for (int i = 0; i < count; ++i) {
        if (i != 0)
            ph += ", ";
        ph += "?";
    }

    statement stat = m_pimpl->SQL << "INSERT OR IGNORE INTO [" + table + "] "
                                     "(" + fields + ") "
                                     "VALUES (" + ph + ");";

    for(int i = 0; i < count; ++i) {
        stat.bind(va_arg(args, char*));
    }

    va_end(args);

    stat.exec();
}

void DB::Update(const std::string &table, const std::string &where, const std::string &value,
                const std::string &set, const int count, ...)
{
    va_list args;
    va_start(args, count);

    statement stat = m_pimpl->SQL << "UPDATE [" + table + "] "
                                     "SET " + set + " "
                                     "WHERE " + where + "=?;";

    for(int i = 0; i < count; ++i) {
        stat.bind(va_arg(args, char*));
    }

    va_end(args);

    stat.bind(value);

    stat.exec();
}

void DB::Delete(const std::string &table, const std::string &where, const std::string &value)
{
    m_pimpl->SQL << "DELETE FROM [" + table + "] "
                    "WHERE " + where + "=?;"
                 << value
                 << exec;
}

DB::Impl::Impl()
{

}

DB::Impl::~Impl()
{

}

