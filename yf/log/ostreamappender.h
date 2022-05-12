#ifndef __YF_OSTREAMAPPENDER_H__
#define __YF_OSTREAMAPPENDER_H__

#include <iostream>

#include "appender.h"

namespace yf {
class OstreamAppender final : public Appender {
public:
    /**
     * @brief 构造一个标准输出/错误Appender
     * @param name Appender名称
     * @param ss 标准输出/错误
     */
    OstreamAppender(const std::string &name, std::ostream &ss = std::cout);
    virtual ~OstreamAppender() = default;
private:
    void _append(LogEvent &&event) override;
    virtual bool reopen() override { return true; }
    virtual void close() override {}
private:
    std::ostream &_ss;
};
}

#endif