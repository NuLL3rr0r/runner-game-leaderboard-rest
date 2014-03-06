#ifndef CORELIB_DB_HPP
#define CORELIB_DB_HPP


#include <string>
#include <cppdb/frontend.h>

namespace CoreLib {
    class DB;
}

class CoreLib::DB
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
#ifdef CORELIB_STATIC
    static void LoadSQLite3Driver();
#endif  // CORELIB_STATIC

    static bool Vacuum(const std::string &dbFile);

public:
    DB(const std::string &dbFile);
    ~DB();

    cppdb::session &SQL();

    void CreateTable(const std::string &table, const std::string &fields);
    void DropTable(const std::string &table);
    void RenameTable(const std::string &table, const std::string &newTable);

    void Insert(const std::string &table, const std::string &fields, const int count, ...);
    void Update(const std::string &table, const std::string &where, const std::string &value,
                const std::string &set, const int count, ...);
    void Delete(const std::string &table, const std::string &where, const std::string &value);
};


#endif /* CORELIB_DB_HPP */

