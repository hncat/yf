#ifndef __YF_CONFIG_H__
#define __YF_CONFIG_H__

#include <string>
#include <memory>
#include <fstream>

#include "mutex.h"
#include "json.h"
#include "noncopyable.h"

namespace yf {
class Config;
class FileAppender;
template<typename F, typename T>
struct LexicalCast;

/**
 * @brief 支持STL各种容器组合
 * @tparam T
 */
template<typename T>
struct LexicalCast<nlohmann::json, T> {
    T operator()(const nlohmann::json &json) const {
        return json.get<T>();
    }
};

template<typename T>
struct LexicalCast<T, nlohmann::json> {
    nlohmann::json operator()(const T &t, const std::string &key = "") const {
        nlohmann::json json;
        json[key] = t;
        return json;
    }
};

class ConfigVarBase : Noncopyable {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    virtual ~ConfigVarBase() = default;
    /**
     * @brief 获取配置名称
     * @return const std::string&
     */
    const std::string &getName() const { return _name; }
    /**
     * @brief 设置配置名称
     * @param name 配置名称
     */
    void setName(const std::string &name) { _name = name; }
    /**
     * @brief 获取配置描述
     * @return const std::string&
     */
    const std::string &getDescription() const { return _description; }
    /**
     * @brief 设置配置描述
     * @param description 配置描述
     */
    void setDescription(const std::string &description) { _description = description; }
    /**
     * @brief 通过json生成对应对象
     * @param json 反序列化json对象
     */
    virtual void formatJson(const nlohmann::json &json) = 0;
    /**
     * @brief 通过对象类型生成json对象
     * @return nlohmann::json
     */
    virtual nlohmann::json toJosn() const = 0;
protected:
    /**
     * @brief 构造配置变量
     * @param name 配置名称
     * @param description 配置描述
     */
    ConfigVarBase(const std::string &name, const std::string &description)
        : _name(name),
          _description(description) {
    }
private:
    std::string _name;        // 配置名称
    std::string _description; // 配置描述
};

template<typename T,
         typename Format = LexicalCast<nlohmann::json, T>,
         typename Tojson = LexicalCast<T, nlohmann::json>>
class ConfigVar : public ConfigVarBase {
    friend class Config;
public:
    typedef T value_type;
    typedef T *value_type_ptr;
    typedef const T const_value_type;
    typedef std::shared_ptr<ConfigVar> ptr;
    /**
     * @brief 设置配置值
     * @param value 配置值
     */
    void setValue(T &&value) { _value = value; }
    /**
     * @brief 获取配置值
     * @return const T&
     */
    const_value_type &getValue() const { return _value; }
    virtual ~ConfigVar() = default;
    virtual void formatJson(const nlohmann::json &json) override {
        _value = Format()(json);
    }
    virtual nlohmann::json toJosn() const override {
        return Tojson()(_value, getName());
    }
private:
    /**
     * @brief 构造配置变量
     * @param value 配置值
     * @param name 配置名称
     * @param description 配置描述
     */
    ConfigVar(const T &value, const std::string &name, const std::string &description)
        : ConfigVarBase(name, description),
          _value(value) {
    }
private:
    T _value; // 配置内容
};

class Config {
public:
    /**
     * @brief 生成配置选项
     * @tparam T
     * @param name 配置名称作为key值
     * @param value 配置值
     * @param description 配置描述
     * @return ConfigVarBase::ptr
     */
    template<typename T>
    static typename ConfigVar<T>::ptr Lookup(const std::string &name, const T &value, const std::string &description = "") {
        SpinLock::Lock lock(_spinLock);
        auto i = _configs.find(name);
        if (i != _configs.end()) {
            return std::dynamic_pointer_cast<ConfigVar<T>>(i->second);
        }
        typename ConfigVar<T>::ptr config(new ConfigVar<T>(value, name, description));
        _configs[name] = config;
        return config;
    }
    /**
     * @brief 从json文件中获取配置信息
     * @param name 配置文件名称
     */
    static void LookupJsonFile(const std::string &name);
private:
    static SpinLock _spinLock;
    static std::map<std::string, ConfigVarBase::ptr> _configs;
};
/**
 * @brief 序列化FileAppender
 * @param json json对象
 * @param appender FileAppender对象
 */
void to_json(nlohmann::json &json, const FileAppender *appender);
/**
 * @brief 反序列化FileAppender
 * @param json json对象
 * @param appender FileAppender对象
 */
void from_json(const nlohmann::json &json, FileAppender *appender);
}

#endif