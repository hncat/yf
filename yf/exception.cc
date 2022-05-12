#include "exception.h"

namespace yf {
LogicError::LogicError(const std::string &ex)
    : std::logic_error(ex) {
}

LogicError::LogicError(const char *ex)
    : std::logic_error(ex) {
}

LogicError::LogicError(LogicError &&ex)
    : std::logic_error(std::forward<std::logic_error>(ex)) {
}

LogicError &LogicError::operator=(LogicError &&ex) {
    std::logic_error *tmp = dynamic_cast<std::logic_error *>(this);
    *tmp = std::forward<std::logic_error>(ex);
    return *this;
}

RuntimeError::RuntimeError(const std::string &ex)
    : std::runtime_error(ex) {
}

RuntimeError::RuntimeError(const char *ex)
    : std::runtime_error(ex) {
}

RuntimeError::RuntimeError(RuntimeError &&ex)
    : std::runtime_error(std::forward<std::runtime_error>(ex)) {
}

RuntimeError &RuntimeError::operator=(RuntimeError &&ex) {
    std::runtime_error *tmp = dynamic_cast<std::runtime_error *>(this);
    *tmp = std::forward<std::runtime_error>(ex);
    return *this;
}
}