#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "UUID.h"

template<typename T>
class MemoryPool
{
public:
    
    void add(UUID uuid, T* resource)
    {
        m_memory[uuid] = resource;
    }

private:
    template<class T>friend class Resource;
    T* get(UUID rid)
    {
        return m_memory[rid];
    }
    std::unordered_map<UUID, T*> m_memory;
    

};