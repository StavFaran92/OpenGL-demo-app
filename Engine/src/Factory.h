#pragma once

#include "UUID.h"

template<typename T> class Resource;

// Partial specialization of Factory for Texture
//template<>
//class Factory<Texture> 
//{
//public:
//    // Specialized createResource function for Texture
//    static Resource<Texture> create() 
//    {
//         Texture* texture = new Texture();
//         auto uid = uuid::generate_uuid_v4();
//         Resource<Texture> res(uid);
//         Engine::get()->getMemoryPool<Texture>()->add(uid, texture);
//         return res;
//    }
//};

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
        return res;
    }
};


