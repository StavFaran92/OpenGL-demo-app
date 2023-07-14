#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

template<typename T>
class MemoryPool
{
public:
    std::shared_ptr<T> get(const std::string& resourceName, std::function<T* ()> creationCallback)
    {
        std::shared_ptr<T> ptr;
        auto it = m_cache.find(resourceName);
        if (it != m_cache.end())
        {
            ptr = it->second.lock();
            if (!ptr)
            {
                m_cache.erase(it);
            }
        }
        else
        {
            ptr = std::shared_ptr<T>(creationCallback());
            m_cache[resourceName] = ptr;
        }

        return ptr;
    }

private:
    std::unordered_map<std::string, std::weak_ptr<T>> m_cache;
};
