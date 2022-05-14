#include <iostream>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <map>
#include <tuple>

#include "log/fileappender.h"
#include "json.h"
#include "config.h"

using namespace nlohmann;

int main(int argc, char **argv) {
    std::string path(argv[0]);
    std::size_t pos = path.find_last_of('/');
    path = path.substr(0, pos);
    ::chdir(path.c_str()); 
    // json j;
    // std::fstream fs("test.json", std::ios::in | std::ios::app);
    // fs >> j;
    // fs.close();
    // using pack = std::tuple<int, std::string, std::string>;
    // auto name = yf::Config::Lookup<std::string>("name", "123", "this is a name");
    // auto pack_conf = yf::Config::Lookup<pack>("tuple", pack());
    // yf::Config::LookupJsonFile("test.json");
    // std::cout << name->getValue() << std::endl;
    yf::FileAppender *appender = new yf::FileAppender("file", "txt.log");
    json j = appender;
    // yf::FileAppender *appender1 = j.get<yf::FileAppender*>();
    // std::cout << appender1 << std::endl;
    return 0;
}