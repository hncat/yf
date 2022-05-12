#ifndef __YF_PATTERNLAYOUT_H__
#define __YF_PATTERNLAYOUT_H__

/**
 * @file patternlayout.h
 * @author yf
 * @brief 自定义格式输出
 * @version 0.1
 * @date 2022-05-06
 * -l: 日志等级
 * -L: 日志行号
 * -t: 线程ID
 * -f: 协程ID
 * -F: 函数名称
 * -m: 日志输出信息
 * -c: 日志分配器名称
 * -d: 日志输出时间
 * -s: 日志当前时间戳
 * -r: 程序运行时长(单位:秒)
 * -n: 换行
 * @copyright Copyright (c) 2022
 */

#include <vector>

#include "layout.h"

namespace yf {
class PatternLayout final : public Layout {
public:
    /**
     * @brief 构造PatternLayout
     * @param format 自定义格式化
     */
    PatternLayout();
    /**
     * @brief 设置自定义格式化
     * @param format 自定义格式
     */
    void setFormat(const std::string &format);
    /**
     * @brief 清除Format
     */
    void clearFormat();
    /**
     * @brief 获取当前的格式化样式
     * @return 当前格式化样式 
     */
    const std::string &getFormat() const { return _format; }
    virtual void format(const LogEvent &event, std::ostream &ss) override;
    virtual ~PatternLayout();
    struct FormatItem {
        typedef std::shared_ptr<FormatItem> ptr;
        FormatItem() = default;
        virtual ~FormatItem() = default;
        virtual void format(const LogEvent &event, std::ostream &ss) = 0;
    };
private:
    std::string _format;                   // 定义的格式化
    std::vector<FormatItem::ptr> _formats; // 格式化集合
};
}

#endif