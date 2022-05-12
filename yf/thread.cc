#include "thread.h"

namespace yf {
static thread_local uint64_t s_thread_id = getThreadId();

Thread::~Thread() {
    if (_joinable) {
        ::pthread_join(_ptid, nullptr);
    }
}

void Thread::join() {
    ::pthread_join(_ptid, nullptr);
    _joinable = false;
}

void Thread::detach() {
    ::pthread_detach(_ptid);
    _joinable = false;
}

void Thread::setName(const std::string &name) {
    _name = name;
    ::pthread_setname_np(_ptid, name.c_str());
}

uint64_t Thread::GetThreadId() {
    return s_thread_id;
}

std::string Thread::GetThreadName() {
    char buf[128] = {};
    ::pthread_getname_np(::pthread_self(), buf, sizeof(buf));
    return std::string(buf);
}
}