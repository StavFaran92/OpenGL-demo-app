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

    void erase(UUID uid)
    {
        if (m_memory.find(uid) == m_memory.end()) return;
        delete m_memory[uid];
        m_memory.erase(uid);
    }
    std::unordered_map<UUID, T*> m_memory;
    

};