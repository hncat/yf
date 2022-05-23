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
    int line = atoi(argv[1]);
    auto category = yf::Category::GetInstance("test");
    yf::Appender *appender1(new yf::FileAppender("file", "txt.log"));
    appender1->setLayout(yf::Layout::ptr(new yf::PatternLayout));
    category->addAppender(appender1);
    // yf::Appender::reopenAll();
    auto begin = ::clock();
    // for (int i = 0; i < 1000000; ++i) {
        // YF_LOG_WARN_FMT(category, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx: %s", "hello");
        YF_LOG_WARN(category) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx: " << "hello";
    // }
    std::cout << ((double)(::clock() - begin)) / CLOCKS_PER_SEC << std::endl;
    return 0;
}
