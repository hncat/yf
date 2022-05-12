#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include "util.h"

namespace yf {
uint64_t getThreadId() {
    return ::syscall(SYS_gettid);
}

uint64_t getCurrMicroSecond() {
    struct timeval tv;
    if (::gettimeofday(&tv, nullptr) == -1) {
        return 0;
    }
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}
}
