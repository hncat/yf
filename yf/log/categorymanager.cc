#include "categorymanager.h"
#include "ostreamappender.h"

namespace yf {
Category *CategoryManager::getInstance(const std::string &name) {
    MutexLock::Lock lock(_mutex);
    return _getInstance(name);
}

Category *CategoryManager::getExistingInstance(const std::string &name) {
    MutexLock::Lock lock(_mutex);
    return _getExistingInstance(name);
}

CategoryManager::~CategoryManager() {
    deleteAllCategory();
}

void CategoryManager::deleteAllCategory() {
    MutexLock::Lock lock(_mutex);
    for (auto &i : _categorys) {
        if (i.second) {
            delete i.second;
        }
    }
    _categorys.clear();
}

Category *CategoryManager::_getInstance(const std::string &name) {
    Category * result = _getExistingInstance(name);
    if (!result) {
        result = new Category(name);
        _categorys[name] = result;
    }
    return result;
}

Category *CategoryManager::_getExistingInstance(const std::string &name) {
    auto i = _categorys.find(name);
    if (i == _categorys.end()) {
        if (name == "" || name == "root") {
            Category *result = new Category("root");
            result->addAppender(new OstreamAppender("ostream", std::cerr));
            _categorys[name] = result;
            return result;
        }
        return nullptr;
    }
    return i->second;
}
}