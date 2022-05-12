#ifndef __YF_CATEGORYMANAGER_H__
#define __YF_CATEGORYMANAGER_H__

#include <memory>
#include <map>

#include "singleton.h"
#include "noncopyable.h"
#include "category.h"
#include "mutex.h"

namespace yf {
class CategoryManager : public std::enable_shared_from_this<CategoryManager> , Noncopyable{
    YF_SINGLETONPTR_FRIEND(CategoryManager)
public:
    /**
     * @brief 获取一个Category
     * 如果集合中不存在这个Category则创建一个实例并返回
     * @param name Category的名称
     * @return Category*
     */
    Category *getInstance(const std::string &name = "");
    /**
     * @brief 从已有实例中获取一个Category
     * @param name Category的名称
     * @return Category*
     */
    Category *getExistingInstance(const std::string &name = "");
    /**
     * @brief Destroy the Category Manager object
     */
    ~CategoryManager();
private:
    CategoryManager() = default;
    /**
     * @brief 删除所有Category
     */
    void deleteAllCategory();
    /**
     * @brief 获取一个实例
     * @param name 实例名称
     * @return Category*
     */
    Category *_getInstance(const std::string &name);
    /**
     * @brief 获取一个已有实例
     * @param name 实例名称
     * @return Category*
     */
    Category *_getExistingInstance(const std::string &name);
private:
    std::map<std::string, Category *> _categorys;
    MutexLock _mutex;
};
typedef SingletonPtr<CategoryManager> CategoryManagerSingleton;
}

#endif