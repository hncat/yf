#ifndef __YF_EXCEPTION_H__
#define __YF_EXCEPTION_H__

#include <stdexcept>

namespace yf {
class LogicError final : public std::logic_error {
public:
    using super_type = std::logic_error;
    explicit LogicError(const std::string &ex);
    explicit LogicError(const char *ex);
    LogicError(const LogicError &ex) = default;
    LogicError &operator=(const LogicError &ex) = default;
    LogicError(LogicError &&ex);
    LogicError &operator=(LogicError &&ex);
    virtual ~LogicError() = default;
};

class RuntimeError final : public std::runtime_error {
public:
    using super_type = std::runtime_error;
    explicit RuntimeError(const std::string &ex);
    explicit RuntimeError(const char *ex);
    RuntimeError(const RuntimeError &ex) = default;
    RuntimeError &operator=(const RuntimeError &ex) = default;
    RuntimeError(RuntimeError &&ex);
    RuntimeError &operator=(RuntimeError &&ex);
    virtual ~RuntimeError() = default;
};
}

#endif