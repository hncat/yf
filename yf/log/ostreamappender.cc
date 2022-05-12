#include "ostreamappender.h"

namespace yf {
OstreamAppender::OstreamAppender(const std::string &name, std::ostream &ss)
    : Appender(name),
      _ss(ss) {
}

void OstreamAppender::_append(LogEvent &&event) {
    _layout->format(event, _ss);
}
}