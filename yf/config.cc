#include "config.h"
#include "log/fileappender.h"
#include "exception.h"
#include "log/category.h"

namespace yf {
SpinLock Config::_spinLock;
std::map<std::string, ConfigVarBase::ptr> Config::_configs;

static void LoadJsonConfig(const nlohmann::json &json, std::map<std::string, ConfigVarBase::ptr> &configs) {
    // 只读取定义的配置信息
    for (const auto &i : configs) {
        auto config_val = json[i.first];
        if (config_val.is_null()) {
            // 如果没有配置则直接忽略
            continue;
        }
        // formatJson
        i.second->formatJson(config_val);
    }
}
void Config::LookupJsonFile(const std::string &name) {
    nlohmann::json json_config;
    SpinLock::Lock lock(_spinLock);
    std::fstream config(name, std::ios::in);
    config >> json_config;
    config.close();
    LoadJsonConfig(json_config, _configs);
}
void to_json(nlohmann::json &json, const FileAppender *appender) {
    json[appender->getName()] = {
        {"filename", appender->getFileName()},
        {"append", appender->getAppend()},
        {"mode", appender->getMode()}
    };
}

void from_json(const nlohmann::json &json, FileAppender *appender) {
    auto i = json.begin();
    if (i == json.end()) {
        return;
    }
    std::string name = i.key();
    appender = dynamic_cast<FileAppender *>(Appender::getAppender(name));
    if (!appender) {
        return;
    }
    auto conf = *i;
    auto filename = conf["filename"];
    if (filename.is_string()) {
        appender->setFileName(filename.get<std::string>());
    } else if (!filename.is_null()) {
        std::string name = filename.type_name();
        std::string exstr = "FileAppender set filename error: type is " + name + ", not std::string";
        throw LogicError(exstr);
    }

    auto append = conf["append"];
    if (append.is_boolean()) {
        appender->setAppend(append.get<bool>());
    } else if (!append.is_null()) {
        std::string name = append.type_name();
        std::string exstr = "FileAppender set append error: type is " + name + ", not bool";
        throw LogicError(exstr);
    }

    auto mode = conf["mode"];
    if (mode.is_number()) {
        appender->setMode(mode);
    } else if (!mode.is_null()) {
        std::string name = mode.type_name();
        std::string exstr = "FileAppender set mode error: type is " + name + ", not number";
        throw LogicError(exstr);
    }
    if (!appender->reopen()) {
        YF_ROOT_LOG_INFO << "FileAppender reopen failed";
    }
}
}