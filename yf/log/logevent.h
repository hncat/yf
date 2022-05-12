#ifndef __YF_LOGEVENT_H__
#define __YF_LOGEVENT_H__

#include "loglevel.h"
#include "timestamp.h"
#include "thread.h"
#include "fiber.h"

namespace yf {
struct LogEvent {
    LogEvent(LogLevel::type level,
             const std::string &message,
             const std::string &categoryNamem)
        : _level(level),
          _threadId(Thread::GetThreadId()),
          _fiberId(Fiber::GetFiberId()),
          _message(std::move(message)),
          _categoryName(categoryNamem) {}

    LogEvent(LogLevel::type Level,
             const char *message,
             const char *categoryName)
        : _level(Level),
          _threadId(Thread::GetThreadId()),
          _fiberId(Fiber::GetFiberId()),
          _message(message),
          _categoryName(categoryName) {}
    
    LogEvent(LogEvent &&event)
      : _level(event._level),
        _threadId(event._threadId),
        _fiberId(event._fiberId),
        _message(std::move(event._message)),
        _categoryName(std::move(event._categoryName)),
        _timeStamp(event._timeStamp.getMicrosecond()) {}

    LogLevel::type _level;     // 日志级别
    uint64_t _threadId;        // 线程ID
    uint64_t _fiberId;         // 协程ID
    std::string _message;      // 日志信息
    std::string _categoryName; // 日志分配器名称
    TimeStamp _timeStamp;      // 时间相关
};
}

#endif
