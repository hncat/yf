#ifndef __YF_TIMESTAMP_H__
#define __YF_TIMESTAMP_H__

#include <inttypes.h>

namespace yf {
class TimeStamp {
public:
    /**
     * @brief 构造当前时间
     */
    TimeStamp();
    /**
     * @brief 构造指定时间戳
     * @param microsecond 微秒
     */
    TimeStamp(uint64_t microsecond);
    /**
     * @brief 获取时间戳微秒
     * @returns 微秒
     */
    uint64_t getMicrosecond() const { return _microsecond; }
    /**
     * @brief 获取时间戳秒
     * @returns 秒
     */
    uint64_t getSecond() const { return _microsecond / (1000 * 1000); }
    /**
     * @brief 获取程序启动的时间
     */
    static const TimeStamp &getStartTime() { return _timeStamp; }
    /**
     * @brief 析构
     */
    ~TimeStamp();
private:
    uint64_t _microsecond;       // 时间戳相关
    static TimeStamp _timeStamp; // 程序第一次运行的时间
};
}

#endif
