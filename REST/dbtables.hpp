#ifndef DBTABLES_HPP
#define DBTABLES_HPP


#include <string>

namespace RossNBoss {
    class DBTables;
}

class RossNBoss::DBTables
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    static void InitTables();

public:
    DBTables();
    ~DBTables();

public:
    std::string Table(const std::string &id);
    std::string Fields(const std::string &id);

    void SetFields(const std::string &tableId, const std::string &fields);
};


#endif /* DBTABLES_HPP */

