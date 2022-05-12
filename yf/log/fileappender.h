#ifndef __YF_FILEAPPENDER_H__
#define __YF_FILEAPPENDER_H__

#include <vector>

#include "appender.h"
#include "thread.h"

namespace yf {
class FileAppender : public Appender {
public:
    /**
     * @brief 创建一个FileAppender
     * @param name Appender名称
     * @param fileName 文件路径
     * @param async 是否异步写入
     * @param maxLine 最大缓冲行数
     * @param second 最大缓冲时间单位: 秒
     * @param append 是否设置为追加模式
     * @param mode 文件权限
     */
    FileAppender(const std::string &name,
                 const std::string &fileName,
                 bool async = false,
                 int maxLine = 100000,
                 int second = 1,
                 bool append = true,
                 mode_t mode = 00644);
    /**
     * @brief 创建一个FileAppender
     * @param name Appender名称
     * @param fd 文件句柄
     */
    FileAppender(const std::string &name, int fd);
    /**
     * @brief 文件是否支持追加属性
     * @param append 是否追加或者截断
     */
    void setAppend(bool append);
    /**
     * @brief 文件是否具有追加属性
     * @return true
     * @return false
     */
    bool getAppend() const;
    /**
     * @brief 设置文件权限 
     * @param mode 文件权限
     */
    void setMode(mode_t mode);
    /**
     * @brief 获取文件权限
     * @return 文件权限
     */
    mode_t getMode() const { return _mode; }
    /**
     * @brief 获取路径
     * @return 文件路径
     */
    const std::string &getFileName() const { return _fileName; }
    /**
     * @brief 获取文件句柄
     * @return 文件句柄
     */
    int getHandler() const { return _fd; }
    virtual bool reopen() override;
    virtual void close() override;
    virtual ~FileAppender();
protected:
    virtual void _append(LogEvent &&event) override;
    virtual void _writeFile(const std::vector<LogEvent> &events, std::stringstream &ss);
private:
    /**
     * @brief 写入日志
     * @param event 日志基本单元
     */
    void _writeToLogFile(LogEvent &&event);
    /**
     * @brief 写入队列
     * @param event 日志基本单元
     */
    void _writeToLogQueue(LogEvent &&event);
    /**
     * @brief 异步写入日志的入口函数
     */
    void _run();
private:
    std::atomic<bool> _isrunning;  // 写入日志线程是否可以执行
    bool _async;                   // 是否异步写入
    int _fd;                       // 文件句柄
    int _flags;                    // 可执行相关
    int _maxLine;                  // 最大缓冲行数
    int _second;                   // 最大缓冲时间
    mode_t _mode;                  // 文件权限
    std::string _fileName;         // 文件路径
    std::vector<LogEvent> _events; // 日志队列
    Thread::ptr _thread;           // 写日志线程
    MutexLock _mutex;              // 用于实现定时和定量写入日志
    CondinitVar _cond;
};
}

#endif
