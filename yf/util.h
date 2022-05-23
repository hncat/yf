#ifndef __YF_UTIL_H__
#define __YF_UTIL_H__

#include <inttypes.h>
#include <string>

namespace yf {
class Layout;
uint64_t getThreadId();
uint64_t getCurrMicroSecond();
Layout *getLayout(const std::string &name);
}

#endif
