#ifndef __YF_LAYOUT_H__
#define __YF_LAYOUT_H__

#include <memory>

#include "logevent.h"

namespace yf {
class Layout : public std::enable_shared_from_this<Layout> {
public:
    typedef std::shared_ptr<Layout> ptr;
    Layout() = default;
    virtual ~Layout() = default;
    /**
     * @brief 格式化输出
     * @param event 日志基本单元
     * @param ss 输出流
     */
    virtual void format(const LogEvent &event, std::ostream &ss) = 0;
};
}
#endif
