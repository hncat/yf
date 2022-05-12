#include <errno.h>
#include <string.h>

#include "mutex.h"
#include "macro.h"
#include "exception.h"

namespace yf {
MutexLock::MutexLock() {
    ::pthread_mutex_init(&_mutex, nullptr);
}

MutexLock::~MutexLock() {
    ::pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock() {
    int err = ::pthread_mutex_lock(&_mutex);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

void MutexLock::unlock() {
    int err = ::pthread_mutex_unlock(&_mutex);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

SpinLock::SpinLock() {
    ::pthread_spin_init(&_spin, PTHREAD_PROCESS_PRIVATE);
}

SpinLock::~SpinLock() {
    ::pthread_spin_destroy(&_spin);
}

void SpinLock::lock() {
    int err = ::pthread_spin_lock(&_spin);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

void SpinLock::unlock() {
    int err = ::pthread_spin_unlock(&_spin);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

AtomicLock::AtomicLock()
    : _atomic{true} {
}

void AtomicLock::lock() {
    bool flag = true;
    while (!_atomic.compare_exchange_weak(flag, false)) {
        flag = true;
    }
}

void AtomicLock::unlock() {
    _atomic.store(true);
}

ReadWriteLock::ReadWriteLock() {
    ::pthread_rwlock_init(&_rwlock, nullptr);
}

ReadWriteLock::~ReadWriteLock() {
    ::pthread_rwlock_destroy(&_rwlock);
}

void ReadWriteLock::rlock() {
    int err = ::pthread_rwlock_rdlock(&_rwlock);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

void ReadWriteLock::wlock() {
    int err = ::pthread_rwlock_wrlock(&_rwlock);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

void ReadWriteLock::unlock() {
    int err = ::pthread_rwlock_unlock(&_rwlock);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

Semaphore::Semaphore(uint32_t num) {
    ::sem_init(&_sem, 0, num);
}

Semaphore::~Semaphore() {
    ::sem_destroy(&_sem);
}

void Semaphore::wait() {
    int err = ::sem_wait(&_sem);
    if (YF_UNLIKELY(err == -1)) {
        throw LogicError(::strerror(errno));
    }
}

void Semaphore::post() {
    int err = ::sem_post(&_sem);
    if (YF_UNLIKELY(err == -1)) {
        throw LogicError(::strerror(errno));
    }
}

CondinitVar::CondinitVar() {
    ::pthread_cond_init(&_cond, nullptr);
}

CondinitVar::~CondinitVar() {
    ::pthread_cond_destroy(&_cond);
}

void CondinitVar::wait(MutexLock::Lock &lock) {
    int err = ::pthread_cond_wait(&_cond, &lock._lock._mutex);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

void CondinitVar::waitForSeconds(MutexLock::Lock &lock, uint32_t second) {
    struct timespec ts;
    ts.tv_nsec = 0;
    ts.tv_sec = second;
    int err = ::pthread_cond_timedwait(&_cond, &lock._lock._mutex, &ts);
    if (err != 0 && err != ETIMEDOUT) {
        throw LogicError(::strerror(err));
    }
}

void CondinitVar::notify_one() {
    int err = ::pthread_cond_signal(&_cond);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}

void CondinitVar::notify_all() {
    int err = ::pthread_cond_broadcast(&_cond);
    if (YF_UNLIKELY(err)) {
        throw LogicError(::strerror(err));
    }
}
}
