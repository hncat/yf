#ifndef __YF_SIMPLELAYOUT_H__
#define __YF_SIMPLELAYOUT_H__

#include "layout.h"

namespace yf {
class SimpleLayout : public Layout {
public:
    SimpleLayout() = default;
    virtual ~SimpleLayout() = default;
    virtual void format(const LogEvent &event, std::ostream &ss);
};
}

#endif