#include <ostream>

#include "passthroughlayout.h"

namespace yf {
void PassThroughLayout::format(const LogEvent &event, std::ostream &ss) {
    ss << event._message << '\n';
}
}