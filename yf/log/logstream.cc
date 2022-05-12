#include "logstream.h"
#include "category.h"

namespace yf {
std::stringstream LogStream::_ss;

LogStream::LogStream(Category &category, LogLevel::type level)
    : _category(category),
      _level(level) {
}

LogStream::~LogStream() {
    flush();
}

void LogStream::flush() {
    _category.log(_level, _ss.str().c_str());
    _ss.str("");
}
}