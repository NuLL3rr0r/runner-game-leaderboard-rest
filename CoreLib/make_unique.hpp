#ifndef CORELIB_MAKE_UNIQUE_HPP
#define CORELIB_MAKE_UNIQUE_HPP


#include <memory>
#include <utility>

namespace std {
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique(Args&& ...args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}


#endif /* CORELIB_MAKE_UNIQUE_HPP */


