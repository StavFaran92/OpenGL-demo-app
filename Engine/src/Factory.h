#pragma once

#include "UUID.h"
#include "ResourceManager.h"

template<typename T> class Resource;

template<typename T>
class Factory
{
public:
    template<typename... Args>
    static Resource<T> create(Args&&... args)
    {
        T* texture = new T();
        auto uid = uuid::generate_uuid_v4();
        Resource<T> res(uid);
        Engine::get()->getMemoryPool<T>()->add(uid, texture);
        Engine::get()->getResourceManager()->incRef(uid);
        return res;
    }
};


