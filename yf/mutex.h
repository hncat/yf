#ifndef __YF_MUTEX_H__
#define __YF_MUTEX_H__

#include <pthread.h>
#include <atomic>
#include <semaphore.h>

#include "noncopyable.h"

namespace yf {
class CondinitVar;

template<typename T>
class ScopedLock final : Noncopyable {
    friend class CondinitVar;
public:
    ScopedLock(T &lock)
        : _lock(lock),
          _is_lock(true) {
        _lock.lock();
    }
    ~ScopedLock() {
        if ( _is_lock) {
            _lock.unlock();
        }
    }
    void lock() {
        if (!_is_lock) {
            _is_lock = true;
            _lock.lock();
        }
    }
    void unlock() {
        if (_is_lock) {
            _lock.unlock();
            _is_lock = false;
        }
    }
private:
    T &_lock;
    bool _is_lock;
};

template<typename T>
class ReadScopedLock final : Noncopyable {
public:
    ReadScopedLock(T &rlock)
        : _rlock(rlock),
          _is_lock(true) {
        rlock.rlock();
    }
    ~ReadScopedLock() {
        if (_is_lock) {
            _rlock.unlock();
        }
    }
    void lock() {
        if (!_is_lock) {
            _is_lock = true;
            _rlock.rlock();
        }
    }
    void unlock() {
        if (_is_lock) {
            _rlock.unlock();
            _is_lock = false;
        }
    }
private:
    T &_rlock;
    bool _is_lock;
};

template<typename T>
class WriteScopedLock final : Noncopyable {
public:
    WriteScopedLock(T &rlock)
        : _wlock(rlock),
          _is_lock(true) {
        rlock.wlock();
    }
    ~WriteScopedLock() {
        if (_is_lock) {
            _wlock.unlock();
        }
    }
    void lock() {
        if (!_is_lock) {
            _is_lock = true;
            _wlock.wlock();
        }
    }
    void unlock() {
        if (_is_lock) {
            _wlock.unlock();
            _is_lock = false;
        }
    }
private:
    T &_wlock;
    bool _is_lock;
};


class MutexLock final : Noncopyable {
    friend class CondinitVar;
public:
    typedef ScopedLock<MutexLock> Lock;
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
private:
    pthread_mutex_t _mutex;
};

class SpinLock final : Noncopyable {
public:
    typedef ScopedLock<SpinLock> Lock;
    SpinLock();
    ~SpinLock();
    void lock();
    void unlock();
private:
    pthread_spinlock_t _spin;
};

class AtomicLock final : Noncopyable {
public:
    typedef ScopedLock<AtomicLock> Lock;
    AtomicLock();
    ~AtomicLock() = default;
    void lock();
    void unlock();
private:
    std::atomic<bool> _atomic;
};

class ReadWriteLock final : Noncopyable {
public:
    typedef ReadScopedLock<ReadWriteLock> ReadLock;
    typedef WriteScopedLock<ReadWriteLock> WriteLock;
    ReadWriteLock();
    ~ReadWriteLock();
    void rlock();
    void wlock();
    void unlock();
private:
    pthread_rwlock_t _rwlock;
};

class Semaphore final : Noncopyable {
public:
    Semaphore(uint32_t num = 0);
    ~Semaphore();
    void wait();
    template<typename Func, typename ...Args>
    void wait(Func cb, Args&& ...args) {
        while (!!cb(std::forward<Args>(args)...)) {
            wait();
        }
    }
    void post();
private:
    sem_t _sem;
};

class CondinitVar final : Noncopyable {
public:
    CondinitVar();
    ~CondinitVar();
    void wait(MutexLock::Lock &lock);
    template<typename Func, typename ...Args>
    void wait(MutexLock::Lock &_cond, Func cb, Args&& ...args) {
        while (!!cb(std::forward<Args>(args)...)) {
            wait(_cond);
        }
    }
    void waitForSeconds(MutexLock::Lock &lock, uint32_t second);
    void notify_one();
    void notify_all();
private:
    pthread_cond_t _cond;
};
}

#endif
