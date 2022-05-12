#ifndef __YF_LOGSTREAM_H__
#define __YF_LOGSTREAM_H__

#include "logevent.h"
#include <sstream>

namespace yf {
class Category;
class LogStream {
public:
    LogStream(Category &category, LogLevel::type level);
    // LogStream(const LogStream &ls) = default;
    ~LogStream();
    void flush();
    template<typename T>
    LogStream &operator<<(T &&t) {
        _ss << std::forward<T>(t);
        return *this;
    }
    template<typename T>
    LogStream &operator<<(const T *t) {
        _ss << t;
        return *this;
    }
private:
    Category &_category;
    LogLevel::type _level;
    static std::stringstream _ss;
};
}

#endif