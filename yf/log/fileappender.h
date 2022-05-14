#ifndef __YF_ASYNCFILEAPPENDER_H__
#define __YF_ASYNCFILEAPPENDER_H__

#include <sstream>

#include "appender.h"
#include "thread.h"

namespace yf {
class FileAppender : public Appender {
public:
    /**
     * @brief 创建一个FileAppender
     * @param name Appender名称
     * @param fileName 文件路径
     * @param append 是否设置为追加模式
     * @param mode 文件权限
     */
    FileAppender(const std::string &name,
                 const std::string &fileName,
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
     * @brief 设置日志名称
     * @param fileName 日志名称
     */
    void setFileName(const std::string &fileName) { _fileName = fileName; }
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
    /**
     * @brief 写入日志
     */
    void _writeToLogFile();
private:
    int _fd;                       // 文件句柄
    int _flags;                    // 可执行相关
    mode_t _mode;                  // 文件权限
    std::string _fileName;         // 文件路径
protected:
    std::stringstream _ss;         // 日志流
};
}

#endif