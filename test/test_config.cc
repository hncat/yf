#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>

#include "config.h"
#include "log/category.h"

int main(int argc, char **argv) {
    std::string path(argv[0]);
    std::size_t pos = path.find_last_of('/');
    path = path.substr(0, pos);
    ::chdir(path.c_str());
    // auto name_conf = yf::Config::Lookup<std::vector<std::vector<int>>>("name", {{1, 2, 4}}, "this is a name");
    // auto name_conf = yf::Config::Lookup<std::forward_list<std::forward_list<int>>>("name", {{1, 2, 4}}, "this is a name");
    // auto name_conf = yf::Config::Lookup<std::forward_list<std::set<int>>>("name", {{1, 2, 4}}, "this is a name");
    auto name_conf = yf::Config::Lookup<std::forward_list<std::unordered_set<int>>>("name", {{1, 2, 4}}, "this is a name");
    auto map_conf = yf::Config::Lookup<std::map<std::string, int>>("map", {}, "this is a map");
    auto tuple_conf = yf::Config::Lookup<std::tuple<std::string, int>>("tuple", {"tome", 22}, "this is a tuple");
    // auto map_conf = yf::Config::Lookup<std::unordered_map<std::string, int>>("map", {}, "this is a map");
    yf::Config::LookupJsonFile("config.json");
    // YF_ROOT_LOG_INFO << name_conf->getValue() << "\n" << name_conf->toJson();
    std::for_each(name_conf->getValue().begin(), name_conf->getValue().end(), [](std::unordered_set<int> a){
        for (const auto &i : a) {
        YF_ROOT_LOG_INFO << i;
        }
    });
    YF_ROOT_LOG_INFO << name_conf->toJson();
    for (auto &i : map_conf->getValue()) {
        YF_ROOT_LOG_INFO << i.first << ":" << i.second;
    }
    YF_ROOT_LOG_INFO << map_conf->toJson();
    YF_ROOT_LOG_INFO << tuple_conf->toJson();
    return 0;
}