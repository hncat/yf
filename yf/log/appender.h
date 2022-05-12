#ifndef __YF_APPENDER_H__
#define __YF_APPENDER_H__

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "noncopyable.h"
#include "mutex.h"
#include "loglevel.h"
#include "logevent.h"
#include "basiclayout.h"

namespace yf {
class Appender : Noncopyable {
public:
    /**
     * @brief 析构
     */
    virtual ~Appender();
    /**
     * @brief 获取Appender的名称
     * @returns Appender的名称
     */
    const std::string &getName() const { return _name;  }
    /**
     * @brief 设置布局
     * @param layout 布局
     */
    void setLayout(Layout::ptr layou);
    /**
     * @brief 设置日志等级
     * @param level 日志等级
     */
    void setLogLevel(LogLevel::type level) { _level = level; }
    /**
     * @brief 获取Appender级别
     * @returns Appender等级
     */
    LogLevel::type getLogLevel() const { return _level; }
    /**
     * @brief 日志输出
     * @param event 日志的基本单元
     */
    void append(LogEvent &&event);
    /**
     * @brief 获取一个指定名称的Appender
     * @param name Appender的名称
     */
    static Appender *getAppender(const std::string &name);
    /**
     * @brief 重新打开全部Appender
     * @return true
     * @return false
     */
    static bool reopenAll();
    /**
     * @brief 关闭全部Appender
     * @return true
     * @return false
     */
    static void closeAll();
protected:
    /**
     * @brief 具体实现由取决于子类
     * @param event 日志基本单元
     */
    virtual void _append(LogEvent &&event) = 0;
public:
    /**
     * @brief 重新打开文件
     */
    virtual bool reopen() = 0;
    /**
     * @brief 关闭文件
     */
    virtual void close() = 0;
private:
    /**
     * @brief 添加一个Appender
     * @param appender 要添加的Appender
     */
    static void addAppender(Appender *appender);
    /**
     * @brief 删除一个Appender
     * @param name Appender的名称
     */
    static void delAppender(const std::string &name);
    /**
     * @brief 删除全部Appender
     */
    static void delAllAppender();
    static void delAllAppenderUnLock(std::vector<Appender *> &appends);
public:
    /**
     * @brief 构造
     * @param name Appender的名称
     */
    Appender(const std::string &name);
protected:
    std::string _name;     // Appender的名称
    Layout::ptr _layout;   // Appender布局
    LogLevel::type _level; // Appender级别
private:
    struct AppenderInitialize {
        AppenderInitialize();
        ~AppenderInitialize();
        std::map<std::string, Appender *> _appenders;
        MutexLock _mutex;
    };
    static AppenderInitialize _appenderInitialize;
};
}

#endif
