#pragma once

#include <unordered_map>
#include <memory>
#include <functional>
#include "UUID.h"
#include "Logger.h"

template<typename T>
class MemoryPool
{
public:
    
    void add(UUID uid, T* resource)
    {
        m_memory[uid] = resource;
    }

private:
    template<class T>friend class Resource;
    T* get(UUID uid)
    {
        return m_memory.at(uid);
    }

    void erase(UUID uid)
    {
        auto iter = m_memory.find(uid);
        if (iter == m_memory.end()) return;
        delete iter->second;
        m_memory.erase(uid);
    }
    std::unordered_map<UUID, T*> m_memory;
    

};