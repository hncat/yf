#include "appender.h"

namespace yf {
Appender::AppenderInitialize Appender::_appenderInitialize;

Appender::AppenderInitialize::AppenderInitialize() {
}

Appender::AppenderInitialize::~AppenderInitialize() {
    Appender::delAllAppender();
}

Appender::~Appender() {
    delAppender(_name);
    _layout = nullptr;
}

void Appender::setLayout(Layout::ptr layout) {
    _layout = layout;
}

void Appender::addAppender(Appender *appender) {
    auto &appenders = _appenderInitialize._appenders;
    MutexLock::Lock lock(_appenderInitialize._mutex);
    appenders[appender->getName()] = appender;
}

void Appender::delAppender(const std::string &name) {
    auto &appenders = _appenderInitialize._appenders;
    MutexLock::Lock lock(_appenderInitialize._mutex);
    auto it = appenders.find(name);
    if (it != appenders.end()) {
        appenders.erase(it);
    }
}

void Appender::delAllAppender() {
    auto &appenders = _appenderInitialize._appenders;
    std::vector<Appender *> vec;
    {
        MutexLock::Lock lock(_appenderInitialize._mutex);
        if (appenders.empty()) {
            return;
        }
        for (auto it = appenders.begin(); it != appenders.end(); ++it) {
            if (it->second) {
                vec.push_back(it->second);
            }
        }
        appenders.clear();
    }
    delAllAppenderUnLock(vec);
}

void Appender::delAllAppenderUnLock(std::vector<Appender *> &appenders) {
    for (auto &it : appenders) {
        if (it) {
            delete it;
        }
    }
}

void Appender::append(LogEvent &&event) {
    if (event._level <= _level) {
        _append(std::forward<LogEvent>(event));
    }
}

Appender *Appender::getAppender(const std::string &name) {
    auto &appenders = _appenderInitialize._appenders;
    MutexLock::Lock lock(_appenderInitialize._mutex);
    auto it = appenders.find(name);
    if (it == appenders.end()) {
        return nullptr;
    }
    return it->second;
}

bool Appender::reopenAll() {
    bool result = true;
    auto &appenders = _appenderInitialize._appenders;
    MutexLock::Lock lock(_appenderInitialize._mutex);
    for (auto &i : appenders) {
        result = result && i.second->reopen();
    }
    return result;
}

void Appender::closeAll() {
    auto &appenders = _appenderInitialize._appenders;
    MutexLock::Lock lock(_appenderInitialize._mutex);
    for (auto &i : appenders) {
        i.second->close();
    }
}

Appender::Appender(const std::string &name)
    : _name(name),
      _layout(new BasicLayout()),
      _level(LogLevel::NOTSET) {
    addAppender(this);
}
}
