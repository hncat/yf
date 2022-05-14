#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

#include "fileappender.h"

namespace yf {
FileAppender::FileAppender(const std::string &name,
                           const std::string &fileName,
                           bool append,
                           mode_t mode)
                           : Appender(name),
                             _fd(-1),
                             _flags(O_CREAT | O_WRONLY | O_APPEND),
                             _mode(mode),
                             _fileName(fileName) {
    if (!append) {
        _flags |= O_TRUNC;
    }
    _fd = ::open(_fileName.c_str(), _flags, _mode);
    assert(_fd >= 0);
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
}

void FileAppender::_append(LogEvent &&event) {
    _layout->format(event, _ss);
    _writeToLogFile();
}

void FileAppender::_writeToLogFile() {
    (void)::write(_fd, _ss.str().c_str(), _ss.str().size());
    _ss.str("");
}
}
