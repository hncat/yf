#ifndef __YF_BASICLAYOUT_H__
#define __YF_BASICLAYOUT_H__

#include "layout.h"

namespace yf {
class BasicLayout : public Layout {
public:
    BasicLayout() = default;
    virtual ~BasicLayout() = default;
    virtual void format(const LogEvent &event, std::ostream &ss);
};
}

#endif