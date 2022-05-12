#ifndef __YF_NONCOPYABLE_H__
#define __YF_NONCOPYABLE_H__

namespace yf {
class Noncopyable {
protected:
    Noncopyable() = default;
    virtual ~Noncopyable() = default;
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};
}

#endif
