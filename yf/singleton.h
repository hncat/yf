#ifndef __YF_SINGLETON_H__
#define __YF_SINGLETON_H__

#include <type_traits>

#define YF_SINGLETON_FRIEND(OBJECT) \
    friend class Singleton<OBJECT>;
#define YF_SINGLETONPTR_FRIEND(OBJECT)   \
public:                                  \
    typedef std::shared_ptr<OBJECT> ptr; \
    friend class SingletonPtr<OBJECT>;

namespace yf {
template<typename T>
class Singleton {
public:
    template<typename... Args>
    static T &get(Args&&... args) {
        static T t(std::forward<Args>(args)...);
        return t;
    }
};

template<typename T, typename = typename std::enable_if<true, typename T::ptr>::type>
class SingletonPtr {
public:
    template<typename... Args>
    static typename T::ptr get(Args&&... args) {
        static typename T::ptr t(new T(std::forward<Args>(args)...));
        return t;
    }
};
}

#endif