#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "UUID.h"

template<typename T>
class MemoryPool
{
public:
    Resource<T> createOrGetCached(const std::string& resourceName, const std::function<T* ()>& creationCallback)
    {
        auto it = m_associations.find(resourceName);
        if (it != m_associations.end())
        {
            auto& uid = it->second;
            Resource<T> resource(uid);
            return resource;
        }

        auto& resource = create(creationCallback);
        m_associations[resourceName] = resource.getUID();

        return resource;
    }
    Resource<T> create(const std::function<T*()>& creationCallback)
    {
        auto obj = creationCallback();
        auto uid = uuid::generate_uuid_v4();
        Resource<T> resource(uid);
        m_cache[uid] = obj;

        return resource;
    }

private:
    template<class T>friend class Resource;
    T* get(const std::string& rid)
    {
        return m_cache[rid];
    }
    std::unordered_map<std::string, T*> m_cache;
    std::unordered_map<std::string, std::string> m_associations;

};