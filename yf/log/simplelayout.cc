#include <ostream>

#include "simplelayout.h"

namespace yf {
void SimpleLayout::format(const LogEvent &event, std::ostream &ss) {
    ss << event._categoryName << ": " << event._message << '\n';
}
}