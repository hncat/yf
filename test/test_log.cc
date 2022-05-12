#include <iostream>
#include <unistd.h>
#include <sstream>

#include "log/loglevel.h"
#include "log/logevent.h"
#include "log/ostreamappender.h"
#include "log/fileappender.h"
#include "log/patternlayout.h"
#include "exception.h"
#include "log/category.h"
#include "log/logstream.h"

int main(int argc, char **argv) {
    std::string path(argv[0]);
    std::size_t pos = path.find_last_of('/');
    path = path.substr(0, pos);
    ::chdir(path.c_str());    
    // std::cout << yf::LogLevel::getLevel("NOTSET") << ":" << yf::LogLevel::getLevelName(yf::LogLevel::NOTSET) << '\n'
    //           << yf::LogLevel::getLevel("DEBUG") << ":" << yf::LogLevel::getLevelName(yf::LogLevel::DEBUG) << '\n'
    //           << yf::LogLevel::getLevel("INFO") << ":" << yf::LogLevel::getLevelName(yf::LogLevel::INFO) << '\n'
    //           << yf::LogLevel::getLevel("WARN") << ":" << yf::LogLevel::getLevelName(yf::LogLevel::WARN) << '\n'
    //           << yf::LogLevel::getLevel("ERROR") << ":" << yf::LogLevel::getLevelName(yf::LogLevel::ERROR) << '\n'
    //           << yf::LogLevel::getLevel("FATAL") << ":" << yf::LogLevel::getLevelName(yf::LogLevel::FATAL) << "\n"
    //           << yf::LogLevel::getLevel("UNKNOW") << ":" << yf::LogLevel::getLevelName((yf::LogLevel::type)(yf::LogLevel::MAX_LEVEL + 1)) << '\n';
    // std::cout << "========================\n";
    // yf::Appender *appender(new yf::OstreamAppender("ostream"));
    // appender->setLayout(yf::Layout::ptr(new yf::PatternLayout));
    // appender->append(event);
    // appender = yf::Appender::getAppender("ostream");
    // appender->append(event);
    int line = atoi(argv[1]);
    auto category = yf::Category::GetInstance("test");
    yf::Appender *appender1(new yf::FileAppender("file", "txt.log", true, line));
    appender1->setLayout(yf::Layout::ptr(new yf::PatternLayout));
    category->addAppender(appender1);
    // yf::Appender::reopenAll();
    auto begin = ::clock();
    for (int i = 0; i < 1000000; ++i) {
        // YF_LOG_WARN_FMT(category, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx: %s", "hello");
        YF_LOG_WARN(category) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx: " << "hello";
    }
    std::cout << ((double)(::clock() - begin)) / CLOCKS_PER_SEC << std::endl;
    return 0;
}
