#include <fstream>

#include "config.h"
#include "log/category.h"

namespace yf {
SpinLock Config::_spinLock;
std::map<std::string, ConfigVarBase::ptr> Config::_configs;

static void LoadJsonConfig(const Json::Value &json, std::map<std::string, ConfigVarBase::ptr> &configs) {
    auto names = json.getMemberNames();
    for (const auto &i : names) {
        auto config_ptr =  configs[i];
        if (config_ptr) {
            config_ptr->formatJson(json[i]);
        }
    }
}

void Config::LookupJsonFile(const std::string &name) {
    std::fstream fs;
    fs.open(name, std::ios::app | std::ios::in);
    if (!fs.is_open()) {
        YF_ROOT_LOG_ERROR << "file open failed! filename: " << name;
        return;
    }
    Json::Reader reader;
    Json::Value root;
    SpinLock::Lock lock(_spinLock);
    reader.parse(fs, root);
    if (!reader.good()) {
        YF_ROOT_LOG_ERROR << "parse json file failed! filename: " << name << ", " << reader.getFormattedErrorMessages();
        return;
    }
    LoadJsonConfig(root, _configs);
}
}