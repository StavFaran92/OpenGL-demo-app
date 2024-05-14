#pragma once

#include <cstdint>
#include <atomic>

class Texture;
class Mesh;
#include "Engine.h"
#include "MemoryPool.h"

template<typename T>
class Resource
{
public:
	static Resource<T> empty;

	Resource() : m_uid(EMPTY_UUID), refCount(std::make_shared<int>(0)) {};

	Resource(std::nullptr_t) : m_uid(EMPTY_UUID), refCount(std::make_shared<int>(0)) {};

	Resource(UUID uid) : m_uid(uid), refCount(std::make_shared<int>(1)) {};

	Resource(const Resource<T>& other) 
	{
		m_uid = other.m_uid;
		refCount = other.refCount;
		if (other.m_uid != EMPTY_UUID)
		{
			(*this->refCount)++;
		}
	};

	Resource<T>& operator=(const Resource<T>& other)
	{
		clean();

		m_uid = other.m_uid;
		refCount = other.refCount;
		if (other.m_uid != EMPTY_UUID)
		{
			(*this->refCount)++;
		}

		return *this;
	};

	Resource(Resource<T>&& other)
	{
		m_uid = other.m_uid;
		refCount = other.refCount;

		other.m_uid = EMPTY_UUID;
		other.refCount = nullptr;
	};

	Resource<T>& operator=(Resource<T>&& other)
	{
		m_uid = other.m_uid;
		refCount = other.refCount;
		
		other.m_uid = EMPTY_UUID;
		other.refCount = nullptr;

		return *this;
	};

	inline T* get() const
	{
		return Engine::get()->getMemoryPool<T>()->get(m_uid);
	}

	inline UUID getUID() const 
	{ 
		return m_uid; 
	}

	void release()
	{
		clean();
	}

	template <class Archive>
	void serialize(Archive& archive) {
		archive(m_uid, refCount);
	}

	~Resource<T>() // destructor
	{
		if(m_uid != EMPTY_UUID) clean();
	}

private:
	void clean()
	{
		(*refCount)--;
		if (*refCount == 0)
		{
			if (m_uid != EMPTY_UUID)
			{
				Engine::get()->getMemoryPool<T>()->erase(m_uid);
			}
			//delete refCount;
			m_uid = EMPTY_UUID;
		}
	}
private:
	UUID m_uid;
	std::shared_ptr<int> refCount;
};