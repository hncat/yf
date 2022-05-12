#ifndef __YF_CATEGORY_H__
#define __YF_CATEGORY_H__

/**
 * @file category.h
 * @author 远方。
 * @brief 日志管理相关。创建出的Category不需要手动释放，由CategoryManager统一管理
 * @version 0.1
 * @date 2022-05-09
 * @copyright Copyright (c) 2022
 * 
 */

#include <memory>

#include "appender.h"
#include "logstream.h"

#define YF_ROOT_CATEGORY yf::Category::GetInstance("root")

#define YF_LOG_NOTSET_FMT(category, fmt, ...) category->notset(fmt, ##__VA_ARGS__)
#define YF_LOG_DEBUG_FMT(category, fmt, ...) category->debug(fmt, ##__VA_ARGS__)
#define YF_LOG_INFO_FMT(category, fmt, ...) category->info(fmt, ##__VA_ARGS__)
#define YF_LOG_WARN_FMT(category, fmt, ...) category->warn(fmt, ##__VA_ARGS__)
#define YF_LOG_ERROR_FMT(category, fmt, ...) category->error(fmt, ##__VA_ARGS__)
#define YF_LOG_FATAL_FMT(category, fmt, ...) category->fatal(fmt, ##__VA_ARGS__)

#define YF_ROOT_LOG_NOTSET_FMT(fmt, ...) \
    YF_LOG_NOTSET_FMT(YF_ROOT_CATEGORY, fmt, ##__VA_ARGS__)
#define YF_ROOT_LOG_DEBUG_FMT(fmt, ...) \
    YF_LOG_DEBUG_FMT(YF_ROOT_CATEGORY, fmt, ##__VA_ARGS__)
#define YF_ROOT_LOG_INFO_FMT(fmt, ...) \
    YF_LOG_INFO_FMT(YF_ROOT_CATEGORY, fmt, ##__VA_ARGS__)
#define YF_ROOT_LOG_WARN_FMT(fmt, ...) \
    YF_LOG_WARN_FMT(YF_ROOT_CATEGORY, fmt, ##_VA_ARGS__)
#define YF_ROOT_LOG_ERROR_FMT(fmt, ...) \
    YF_LOG_ERROR_FMT(YF_ROOT_CATEGORY, fmt, ##__VA_ARGS__)
#define YF_ROOT_LOG_FATAL_FMT(fmt, ...) \
    YF_LOG_FATAL_FMT(YF_ROOT_CATEGORY, fmt, ##__VA_ARGS__)

#define YF_LOG_NOTSET(category) (*category) << yf::LogLevel::NOTSET
#define YF_LOG_DEBUG(category) (*category) << yf::LogLevel::DEBUG
#define YF_LOG_INFO(category) (*category) << yf::LogLevel::INFO
#define YF_LOG_WARN(category) (*category) << yf::LogLevel::WARN
#define YF_LOG_ERROR(category) (*category) << yf::LogLevel::ERROR
#define YF_LOG_FATAL(category) (*category) << yf::LogLevel::FATAL

#define YF_ROOT_LOG_NOTSET YF_LOG_NOTSET(YF_ROOT_CATEGORY)
#define YF_ROOT_LOG_DEBUG YF_LOG_DEBUG(YF_ROOT_CATEGORY)
#define YF_ROOT_LOG_INFO YF_LOG_INFO(YF_ROOT_CATEGORY)
#define YF_ROOT_LOG_WARN YF_LOG_WARN(YF_ROOT_CATEGORY)
#define YF_ROOT_LOG_ERROR YF_LOG_ERROR(YF_ROOT_CATEGORY)
#define YF_ROOT_LOG_FATAL YF_LOG_FATAL(YF_ROOT_CATEGORY)

namespace yf {
class Category {
    friend class CategoryManager;
    friend class LogStream;
public:
    /**
     * @brief 获取一个Category
     * 如果不存在则创建
     * @param name 名称 key值
     * @return Category*
     */
    static Category *GetInstance(const std::string &name = "");
    /**
     * @brief 获取一个Category
     * 如果不存在则返回空
     * @param name 名称 可以值
     * @return Category* 
     */
    static Category *GetExistingInstance(const std::string &name = "");
public:
    /**
     * @brief 设置Category等级限制
     * @param level 等级限制
     */
    void setLevel(LogLevel::type level) { _level = level; }
    /**
     * @brief 获取Category等级限制
     * @return Level
     */
    LogLevel::type getLevel() const { return _level; }
    /**
     * @brief 添加一个Appender日志输出
     * @param appender Appender对象
     */
    void addAppender(Appender *appender);
    /**
     * @brief C风格的日志输出
     * 日志等级NONE
     * @param fmt 自定义输出格式
     * @param ... 自定义输出参数
     */
    void notset(const char *fmt, ...);
    /**
     * @brief C风格的日志输出
     * 日志等级DEBUG
     * @param fmt 自定义输出格式
     * @param ... 自定义输出参数
     */
    void debug(const char *fmt, ...);
    /**
     * @brief C风格的日志输出
     * 日志等级INFO
     * @param fmt 自定义输出格式
     * @param ... 自定义输出参数
     */
    void info(const char *fmt, ...);
    /**
     * @brief C风格的日志输出
     * 日志等级WARN
     * @param fmt 自定义输出格式
     * @param ... 自定义输出参数
     */
    void warn(const char *fmt, ...);
    /**
     * @brief C风格的日志输出
     * 日志等级ERROR
     * @param fmt 自定义输出格式
     * @param ... 自定义输出参数
     */
    void error(const char *fmt, ...);
    /**
     * @brief C风格的日志输出
     * 日志等级FATAL
     * @param fmt 自定义输出格式
     * @param ... 自定义输出参数
     */
    void fatal(const char *fmt, ...);
    /**
     * @brief 提供C++风格的流式日志输出
     * @param level 日志等级
     * @return LogStream
     */
    LogStream getLogStream(LogLevel::type level);
    /**
     * @brief 提供C++风格的流式日志输出
     * @param level 日志等级
     * @return LogStream
     */
    LogStream operator<<(LogLevel::type level);
private:
    /**
     * @brief 构造Category
     * @param name Category的名称
     */
    Category(const std::string &name);
    /**
     * @brief C风格的日志输出
     * 进一步封装
     * @param level 日志等级d
     * @param fmt 自定义输出格式
     * @param vl 自定义输出参数
     */
    void logva(LogLevel::type level, const char *fmt, va_list vl);
    /**
     * @brief C++流式风格日志输出
     * @param level 日志等级
     * @param message 用户输出信息
     */
    void log(LogLevel::type level, const char *message);
private:
    LogLevel::type _level;
    std::string _name;
    std::vector<Appender *>_appenders;
};
}

#endif