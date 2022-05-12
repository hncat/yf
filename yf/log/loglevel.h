#ifndef __YF_LOGLEVEL_H__
#define __YF_LOGLEVEL_H__

#include <string>

namespace yf {
class LogLevel {
public:
    typedef enum {
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        NOTSET
    } Level;
    typedef Level type;
    static uint8_t MAX_LEVEL;
    static const std::string &getLevelName(Level level);
    static Level getLevel(const std::string &level);
};
}

#endif
