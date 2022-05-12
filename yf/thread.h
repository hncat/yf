#ifndef __YF_THREAD_H__
#define __YF_THREAD_H__

#include <memory>
#include <tuple>
#include <pthread.h>
#include <set>
#include <type_traits>
#include <string>
#include <assert.h>

#include "noncopyable.h"
#include "mutex.h"
#include "util.h"
#include "macro.h"
#include "exception.h"

namespace yf {
template<typename... _Tp>
using decay_tuple = std::tuple<typename std::decay<_Tp>::type...>;

template<typename Func, typename... Args, size_t... I>
void appaly(std::tuple<Func, Args...> &arg_pack, std::index_sequence<I...>) {
    (std::get<0>(arg_pack))(std::forward<Args>(std::get<I + 1>(arg_pack))...);
}

template<typename Func, typename... Args>
void appaly(std::tuple<Func, Args...> &arg_pack) {
    appaly(arg_pack, std::make_index_sequence<sizeof...(Args)>());
}

template<typename Tp, typename Cp, typename... Args, size_t... I>
void appaly(std::tuple<Tp Cp::*, Cp *, Args...> &arg_pack, std::index_sequence<I...>) {
    (std::get<1>(arg_pack)->*(std::get<0>(arg_pack)))(std::forward<Args>(std::get<I + 2>(arg_pack))...);
}

template<typename Tp, typename Cp, typename... Args>
void appaly(std::tuple<Tp Cp::*, Cp *, Args...> &arg_pack) {
    appaly(arg_pack, std::make_index_sequence<sizeof...(Args)>());
}

class Thread : Noncopyable {
public:
    typedef std::shared_ptr<Thread> ptr;
    /**
     * @brief Construct a new Thread object
     * @tparam Func
     * @tparam Args
     * @param func 线程入口函数
     * @param args 函数参数
     */
    template<typename Func, typename... Args>
    Thread(Func &&func, Args&&... args)
        : _joinable(false),
          _ptid(0),
          _tid(0),
          _name("unknow") {
        static decay_tuple<Func, Args...> *arg_pack = new decay_tuple<Func, Args...>(std::forward<Func>(func), std::forward<Args>(args)...);
        struct PackHandle {
            static void *run(void *arg) {
                Thread *t = static_cast<Thread *>(arg);
                if (t) {
                    t->_tid = yf::getThreadId();
                    t->_sem.post();
                    ::pthread_setname_np(t->_ptid, t->_name.c_str());
                }
                t->_joinable = true;
                try {
                    appaly(*arg_pack);
                } catch (...) {
                    delete arg_pack;
                    throw RuntimeError("appaly running error!");
                }
                delete arg_pack;
                ::pthread_exit(nullptr);
            }
        };
        int err = ::pthread_create(&_ptid, nullptr, &PackHandle::run, this);
        if (YF_UNLIKELY(err)) {
            assert(err != 0);
            return;
        }
        _sem.wait([this]()->bool{
            return _tid == 0;
        });
    }
    /**
     * @brief Destroy the Thread object
     */
    ~Thread();
    /**
     * @brief 是否可以等待线程执行完成
     * @return true
     * @return false
     */
    bool joinable() const { return _joinable; }
    /**
     * @brief 等待线程执行完成
     */
    void join();
    /**
     * @brief 执行后台线程
     */
    void detach();
    /**
     * @brief 获取线程ID
     * @return uint64_t
     */
    uint64_t getThreadId() const {
        return _tid;
    }
    /**
     * @brief 设置线程名称
     * @param name 线程名称
     */
    void setName(const std::string &name = "");
    /**
     * @brief 获取线程名称
     * @return const std::string&
     */
    const std::string &getName() const { return _name; }
    /**
     * @brief 获取线程ID
     * @return uint64_t
     */
    static uint64_t GetThreadId();
    static std::string GetThreadName();
private:
    std::atomic<bool> _joinable;
    pthread_t _ptid;
    uint64_t _tid;
    Semaphore _sem;
    std::string _name;
};
}

#endif