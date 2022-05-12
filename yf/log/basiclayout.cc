#include <ostream>

#include "basiclayout.h"

namespace yf {
void BasicLayout::format(const LogEvent &event, std::ostream &ss) {
    ss << event._timeStamp.getSecond() << " "
       << LogLevel::getLevelName(event._level) << " "
       << event._categoryName << ": " << event._message << '\n';
}
}