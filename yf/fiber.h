#ifndef __YF_FIBER_H__
#define __YF_FIBER_H__

#include <inttypes.h>

namespace yf {
class Fiber {
public:
    static uint64_t GetFiberId();
};
}

#endif