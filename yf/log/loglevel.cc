#include "loglevel.h"

namespace yf {
uint8_t LogLevel::MAX_LEVEL = LogLevel::NOTSET;

#define YF_LEVEL_STR(LEVEL) #LEVEL
static std::string LogLevelName[] = {
    YF_LEVEL_STR(FATAL),
    YF_LEVEL_STR(ERROR),
    YF_LEVEL_STR(WARN),
    YF_LEVEL_STR(INFO),
    YF_LEVEL_STR(DEBUG),
    YF_LEVEL_STR(NOTSET)
};
#undef YF_LEVEL_STR

const std::string &LogLevel::getLevelName(LogLevel::type level) {
    if (level > MAX_LEVEL || level < 0) {
        return LogLevelName[MAX_LEVEL];
    }
    return LogLevelName[level];
}

LogLevel::type LogLevel::getLevel(const std::string &level) {
    for (size_t i = 0; i < MAX_LEVEL; ++i) {
        if (LogLevelName[i] == level) {
            return static_cast<LogLevel::type>(i);
        }
    }
    return static_cast<LogLevel::type>(MAX_LEVEL);
}
}
