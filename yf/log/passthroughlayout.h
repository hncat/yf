#ifndef __YF_PASSTHROUGHLAYOUT_H__
#define __YF_PASSTHROUGHLAYOUT_H__

#include "layout.h"

namespace yf {
class PassThroughLayout : public Layout {
public:
    PassThroughLayout() = default;
    virtual ~PassThroughLayout() = default;
    virtual void format(const LogEvent &event, std::ostream &ss);
};
}

#endif