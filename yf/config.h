#ifndef __YF_CONFIG_H__
#define __YF_CONFIG_H__

#include <string>
#include <memory>
#include <fstream>
#include <algorithm>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <tuple>
#include <type_traits>

#include "mutex.h"
#include "noncopyable.h"
#include "json/json.h"
#include "exception.h"
#include "log/category.h"

namespace yf {
class Config;
template<typename F, typename T>
struct LexicalCast;

// 仅支持基本类型转换(包括std::string)
template<typename T>
struct LexicalCast<Json::Value, T> {
    T operator()(const Json::Value &json) const {
        return json.as<T>();
    }
};

template<typename T>
struct LexicalCast<T, Json::Value> {
    Json::Value operator()(const std::string &name, const T &t) const {
        Json::Value json;
        if (name.empty()) {
            json = t;
        } else {
            json[name] = t;
        }
        return json;
    }
};

// stl各类容器支持实现
template<typename T>
struct LexicalCast<Json::Value, std::vector<T>> {
    std::vector<T> operator()(const Json::Value &json) const {
        std::vector<T> vec;
        if (!json.isArray()) {
            std::stringstream ss;
            ss << "json is not array, json date: " << json;
            throw LogicError(ss.str());
        }
        for (const auto &i : json) {
            vec.push_back(LexicalCast<Json::Value, T>()(i));
        }
        return vec;
    }
};

template<typename T>
struct LexicalCast<std::vector<T>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::vector<T> &vec) const {
        Json::Value json;
        for (const auto &i : vec) {
            if (name.empty()) {
                json.append(LexicalCast<T, Json::Value>()("", i));
            } else {
                json[name].append(LexicalCast<T, Json::Value>()("", i));
            }
        }
        return json;
    }
};

template<typename T>
struct LexicalCast<Json::Value, std::list<T>> {
    std::list<T> operator()(const Json::Value &json) const {
        std::list<T> lst;
        if (!json.isArray()) {
            std::stringstream ss;
            ss << "json is not array, json date: " << json;
            throw LogicError(ss.str());
        }
        for (const auto &i : json) {
            lst.push_back(LexicalCast<Json::Value, T>()(i));
        }
        return lst;
    }
};

template<typename T>
struct LexicalCast<std::list<T>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::list<T> &lst) const {
        Json::Value json;
        for (const auto &i : lst) {
            if (name.empty()) {
                json.append(LexicalCast<T, Json::Value>()("", i));
            } else {
                json[name].append(LexicalCast<T, Json::Value>()("", i));
            }
        }
        return json;
    }
};

template<typename T>
struct LexicalCast<Json::Value, std::forward_list<T>> {
    std::forward_list<T> operator()(const Json::Value &json) const {
        std::forward_list<T> lst;
        if (!json.isArray()) {
            std::stringstream ss;
            ss << "json is not array, json date: " << json;
            throw LogicError(ss.str());
        }
        for (const auto &i : json) {
            lst.push_front(LexicalCast<Json::Value, T>()(i));
        }
        lst.reverse();
        return lst;
    }
};

template<typename T>
struct LexicalCast<std::forward_list<T>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::forward_list<T> &lst) const {
        Json::Value json;
        for (const auto &i : lst) {
            if (name.empty()) {
                json.append(LexicalCast<T, Json::Value>()("", i));
            } else {
                json[name].append(LexicalCast<T, Json::Value>()("", i));
            }
        }
        return json;
    }
};

template<typename T>
struct LexicalCast<Json::Value, std::set<T>> {
    std::set<T> operator()(const Json::Value &json) const {
        std::set<T> s;
        if (!json.isArray()) {
            std::stringstream ss;
            ss << "json is not array, json date: " << json;
            throw LogicError(ss.str());
        }
        for (const auto &i : json) {
            s.insert(LexicalCast<Json::Value, T>()(i));
        }
        return s;
    }
};

template<typename T>
struct LexicalCast<std::set<T>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::set<T> &s) const {
        Json::Value json;
        for (const auto &i : s) {
            if (name.empty()) {
                json.append(LexicalCast<T, Json::Value>()("", i));
            } else {
                json[name].append(LexicalCast<T, Json::Value>()("", i));
            }
        }
        return json; 
    }
};

template<typename T>
struct LexicalCast<Json::Value, std::unordered_set<T>> {
    std::unordered_set<T> operator()(const Json::Value &json) const {
        std::unordered_set<T> us;
        if (!json.isArray()) {
            std::stringstream ss;
            ss << "json is not array, json date: " << json;
            throw LogicError(ss.str());
        }
        for (const auto &i : json) {
            us.insert(LexicalCast<Json::Value, T>()(i));
        }
        return us;
    }
};

template<typename T>
struct LexicalCast<std::unordered_set<T>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::unordered_set<T> &us) const {
        Json::Value json;
        for (const auto &i : us) {
            if (name.empty()) {
                json.append(LexicalCast<T, Json::Value>()("", i));
            } else {
                json[name].append(LexicalCast<T, Json::Value>()("", i));
            }
        }
        return json; 
    }
};

template<typename T>
struct LexicalCast<Json::Value, std::map<std::string, T>> {
    std::map<std::string, T> operator()(const Json::Value &json) const {
        std::map<std::string, T> m;
        if (!json.isObject()) {
            std::stringstream ss;
            ss << "json is not object, json date: " << json;
            throw LogicError(ss.str());
        }
        auto keys = json.getMemberNames();
        for (auto &key : keys) {
            m[key] = LexicalCast<Json::Value, T>()(json[key]);
        }
        return m;
    }
};

template<typename T>
struct LexicalCast<std::map<std::string, T>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::map<std::string, T> &m) const {
        Json::Value json;
        Json::Value result;
        for (const auto &i : m) {
            json[i.first] = LexicalCast<T, Json::Value>()("", i.second);
        }
        if (!name.empty()) {
            result[name] = json;
        }
        return result; 
    }
};

template<typename T>
struct LexicalCast<Json::Value, std::unordered_map<std::string, T>> {
    std::unordered_map<std::string, T> operator()(const Json::Value &json) const {
        std::unordered_map<std::string, T> um;
        if (!json.isObject()) {
            std::stringstream ss;
            ss << "json is not object, json date: " << json;
            throw LogicError(ss.str());
        }
        auto keys = json.getMemberNames();
        for (auto &key : keys) {
            um[key] = LexicalCast<Json::Value, T>()(json[key]);
        }
        return um;
    }
};

template<typename T>
struct LexicalCast<std::unordered_map<std::string, T>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::unordered_map<std::string, T> &um) const {
        Json::Value json;
        Json::Value result;
        for (const auto &i : um) {
            json[i.first] = LexicalCast<T, Json::Value>()("", i.second);
        }
        if (!name.empty()) {
            result[name] = json;
        }
        return result; 
    }
};

struct HelpersLexicalCast {
    template<typename _Ty>
    void operator()(_Ty &v, const Json::Value &json) {
        v = LexicalCast<Json::Value, _Ty>()(json);
    }
    template<typename _Ty>
    void operator()(Json::Value &json, const _Ty &v) {
        json.append(LexicalCast<_Ty, Json::Value>()("", v));
    }
};

template<typename _Ty, size_t _N>
struct TupleLexicalCast {
    void operator()(_Ty &t, const Json::Value &json) const {
        auto val = std::get<_N - 1>(t);
        uint32_t i = _N - 1;
        HelpersLexicalCast()(val, json[i]);
        std::get<_N - 1>(t) = val;
        TupleLexicalCast<_Ty, _N - 1>()(t, json);
    }
    void operator()(Json::Value &json, const _Ty &t) {
        HelpersLexicalCast()(json, std::get<_N - 1>(t));
        TupleLexicalCast<_Ty, _N - 1>()(json, t);
    }
};

template<typename _Ty>
struct TupleLexicalCast<_Ty, 1> {
    void operator()(_Ty &t, const Json::Value &json) const {
        auto val = std::get<0>(t);
        HelpersLexicalCast()(val, json[0]);
        std::get<0>(t) = val;
    }
    void operator()(Json::Value &json, const _Ty &t) const {
        HelpersLexicalCast()(json, std::get<0>(t));
    }
};


template<typename... Args>
struct LexicalCast<Json::Value, std::tuple<Args...>> {
    std::tuple<Args...> operator()(const Json::Value &json) const {
        std::tuple<Args...> t;
        if (!json.isArray()) {
            std::stringstream ss;
            ss << "json is not array, json date: " << json;
            throw LogicError(ss.str());
        }
        TupleLexicalCast<decay_tuple<Args...>, sizeof...(Args)>()(t, json);
        return t;
    }
};

template<typename... Args>
struct LexicalCast<std::tuple<Args...>, Json::Value> {
    Json::Value operator()(const std::string &name, const std::tuple<Args...> &t) const {
        Json::Value result;
        TupleLexicalCast<decay_tuple<Args...>, sizeof...(Args)>()(result[name], t);
        return result; 
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
    const std::string &getName() const {
        SpinLock::Lock lock(_lock);
        return _name;
    }
    /**
     * @brief 设置配置名称
     * @param name 配置名称
     */
    void setName(const std::string &name) {
        SpinLock::Lock lock(_lock);
        _name = name;
    }
    /**
     * @brief 获取配置描述
     * @return const std::string&
     */
    const std::string &getDescription() const {
        SpinLock::Lock lock(_lock);
        return _description;
    }
    /**
     * @brief 设置配置描述
     * @param description 配置描述
     */
    void setDescription(const std::string &description) {
        SpinLock::Lock lock(_lock);
        _description = description;
    }
    /**
     * @brief 通过json生成对应对象
     * @param json 反序列化json对象
     */
    virtual void formatJson(const Json::Value &json) = 0;
    /**
     * @brief 通过对象类型生成json对象
     * @return Json::Value
     */
    virtual Json::Value toJson() const = 0;
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
protected:
    std::string _name;        // 配置名称
    std::string _description; // 配置描述
    mutable SpinLock _lock;
};

template<typename T,
         typename Format = LexicalCast<Json::Value, T>,
         typename Tojson = LexicalCast<T, Json::Value>>
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
    void setValue(T &&value) {
        SpinLock::Lock lock(_lock);
        _value = value;
    }
    /**
     * @brief 获取配置值
     * @return const_value_type&
     */
    const_value_type &getValue() const {
        SpinLock::Lock lock(_lock);
        return _value;
    }
    virtual ~ConfigVar() = default;
    virtual void formatJson(const Json::Value &json) override {
        SpinLock::Lock lock(_lock);
        _value = Format()(json);
    }
    virtual Json::Value toJson() const override {
        SpinLock::Lock lock(_lock);
        return Tojson()(_name, _value);
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
        std::string tmp_name = name;
        std::transform(tmp_name.begin(), tmp_name.end(), tmp_name.begin(), ::tolower);
        std::size_t pos = 0;
        if ((pos = tmp_name.find_first_not_of("abcdefghijkmnlopqrstuvwxyz_0123456789")) != std::string::npos) {
            throw LogicError(std::string("Lookup error, index: ") + std::to_string(pos));
        }
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
     * @brief 查找配置
     * @tparam T
     * @param name 配置名称
     * @return ConfigVar<T>::ptr
     */
    template<typename T>
    static typename ConfigVar<T>::ptr Lookup(const std::string &name) {
        SpinLock::Lock lock(_spinLock);
        auto i = _configs.find(name);
        if (i == _configs.end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(i->second);
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
}

#endif