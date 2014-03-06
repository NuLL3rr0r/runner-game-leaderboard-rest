#ifndef SERVICE_CONTRACT_HPP
#define SERVICE_CONTRACT_HPP


#include <vector>

namespace RossNBoss {
    class ServiceContract;
}

class RossNBoss::ServiceContract
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    ServiceContract();
    ~ServiceContract();

public:
    void Register(const std::wstring &uriTemplate);
    bool Resolve(const std::wstring &uri, std::wstring &out_uriTemplate,
                 std::vector<std::wstring> &out_args) const;
};


#endif /* SERVICE_CONTRACT_HPP */


