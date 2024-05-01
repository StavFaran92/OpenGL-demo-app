#pragma once

#include "Texture.h"

template<typename T> class Resource;

// Partial specialization of Factory for Texture
template<>
class Factory<Texture> 
{
public:
    // Specialized createResource function for Texture
    static Resource<Texture> create() {
        return Engine::get()->getMemoryManagementSystem()->m_texturePool.create([]() { return new Texture(); });
    }
};

template<typename T>
class Factory
{
public:
    template<typename... Args>
    static Resource<T> create(Args&&... args){};
};


