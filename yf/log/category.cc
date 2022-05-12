#include <stdarg.h>

#include "category.h"
#include "categorymanager.h"

namespace yf {
Category *Category::GetInstance(const std::string &name) {
    return CategoryManagerSingleton::get()->getInstance(name);
}

Category *Category::GetExistingInstance(const std::string &name) {
    return CategoryManagerSingleton::get()->getExistingInstance(name);
}

void Category::addAppender(Appender *appender) {
    _appenders.push_back(appender);
}

void Category::notset(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    logva(LogLevel::NOTSET, fmt, vl);
    va_end(vl);
}

void Category::debug(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    logva(LogLevel::DEBUG, fmt, vl);
    va_end(vl);
}

void Category::info(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    logva(LogLevel::INFO, fmt, vl);
    va_end(vl);
}

void Category::warn(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    logva(LogLevel::WARN, fmt, vl);
    va_end(vl);
}

void Category::error(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    logva(LogLevel::ERROR, fmt, vl);
    va_end(vl);
}

void Category::fatal(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    logva(LogLevel::FATAL, fmt, vl);
    va_end(vl);
}

LogStream Category::getLogStream(LogLevel::type level) {
    return LogStream(*this, level);
}

LogStream Category::operator<<(LogLevel::type level) {
    return LogStream(*this, level);
}

Category::Category(const std::string &name)
    : _level(yf::LogLevel::NOTSET),
      _name(name) {
}

void Category::logva(LogLevel::type level, const char *fmt, va_list vl) {
    if (_level != LogLevel::NOTSET && level > _level) {
        return;
    }
    char *buf = nullptr;
    ::vasprintf(&buf, fmt, vl);
    for (auto &i : _appenders) {
        i->append(LogEvent(level, buf, _name.c_str()));
    }
    ::free(buf);
}

void Category::log(LogLevel::type level, const char *message) {
    if (_level == LogLevel::NOTSET || level <= _level) {
        for (auto &i : _appenders) {
            i->append(LogEvent(level, message, _name.c_str()));
        }
    }
}
}