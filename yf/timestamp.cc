#include "timestamp.h"
#include "util.h"

namespace yf {
TimeStamp TimeStamp::_timeStamp;

TimeStamp::TimeStamp()
    : _microsecond(getCurrMicroSecond()) {
}

TimeStamp::TimeStamp(uint64_t microsecond)
    : _microsecond(microsecond) {
}

TimeStamp::~TimeStamp() {
}
}
