#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sstream>

#include "fileappender.h"

namespace yf {
FileAppender::FileAppender(const std::string &name,
                           const std::string &fileName,
                           bool async,
                           int maxLine,
                           int second,
                           bool append,
                           mode_t mode)
                           : Appender(name),
                             _isrunning(false),
                             _async(async),
                             _fd(-1),
                             _flags(O_CREAT | O_WRONLY | O_APPEND),
                             _maxLine(maxLine),
                             _second(second),
                             _mode(mode),
                             _fileName(fileName),
                             _thread(nullptr)
                             {
    if (!append) {
        _flags |= O_TRUNC;
    }
    _fd = ::open(_fileName.c_str(), _flags, _mode);
    assert(_fd >= 0);
    if (_async) {
        _events.reserve(32);
        _isrunning = true;
        _thread = Thread::ptr(new Thread(&FileAppender::_run, this));
    }
}

FileAppender::FileAppender(const std::string &name, int fd)
    : Appender(name),
      _fd(fd),
      _flags(O_CREAT | O_WRONLY | O_APPEND),
      _mode(00644),
      _fileName("") {
    assert(fd >= 0);
}

void FileAppender::setAppend(bool append) {
    if (append) {
        _flags &= ~O_TRUNC;
    } else {
        _flags |= O_TRUNC;
    }
}

bool FileAppender::getAppend() const {
    return (_flags & O_TRUNC) == 0;
}

void FileAppender::setMode(mode_t mode) {
    _mode = mode;
}

bool FileAppender::reopen() {
    if (_fileName != "") {
        int fd = ::open(_fileName.c_str(), _flags, _mode);
        if (fd >= 0) {
            if (_fd >= 0) {
                ::close(_fd);
            }
            _fd = fd;
            return true;
        }
    } else {
        return true;
    }
    return false;
}

void FileAppender::close() {
    ::close(_fd);
    _fd = -1;
}

FileAppender::~FileAppender() {
    if (_async) {
        _cond.notify_one();
        _isrunning = false;
        _thread = nullptr; // 强制等待日志写入线程执行完成
    }
    close();
}

void FileAppender::_append(LogEvent &&event) {
    if (_async) {
        _writeToLogQueue(std::forward<LogEvent>(event));
    } else {
        _writeToLogFile(std::forward<LogEvent>(event));
    }
}

void FileAppender::_writeFile(const std::vector<LogEvent> &events, std::stringstream &ss) {
    for (const auto &event : events) {
        _layout->format(event, ss);
    }
    (void)::write(_fd, ss.str().c_str(), ss.str().size());
}

void FileAppender::_writeToLogFile(LogEvent &&event) {
    std::stringstream ss;
    _layout->format(event, ss);
    (void)::write(_fd, ss.str().c_str(), ss.str().size());
}

void FileAppender::_writeToLogQueue(LogEvent &&event) {
    MutexLock::Lock lock(_mutex);
    _events.push_back(std::forward<LogEvent>(event));
    if (_events.size() >= (std::size_t)_maxLine) {
        _cond.notify_one();
    }
}

void FileAppender::_run() {
    std::vector<LogEvent> events;
    std::stringstream ss;
    while (_isrunning) {
        {
            MutexLock::Lock lock(_mutex);
            while (_events.size() < (std::size_t)_maxLine && _isrunning) {
                _cond.waitForSeconds(lock, _second);
            }
            events.swap(_events);
        }
        _writeFile(events, ss);
        events.clear();
        ss.str("");
    }
    if (!_events.empty()) {
        _writeFile(_events, ss);
    }
}
}
