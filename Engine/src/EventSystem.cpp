#include "EventSystem.h"

uint64_t EventSystem::addEventListener(SDL_EventType eventType, const std::function<void(SDL_Event e)>& callback)
{
	if (m_listeners.find(eventType) == m_listeners.end())
	{
		m_listeners[eventType] = {};
	}

	uint64_t listenterID = ++s_listeners;

	m_listeners[eventType].insert(listenterID);

	m_callbacks[listenterID] = { true, callback };

	return listenterID;
}

void EventSystem::addEventListener(SDL_EventType eventType, const std::function<void(SDL_Event e)>& callback, std::shared_ptr<Handler>& handler)
{
	addEventListener(eventType, callback);
}

void EventSystem::removeEventListener(uint64_t listenerID)
{
	auto iter = m_callbacks.find(listenerID);
	if (iter == m_callbacks.end()) 
		return;

	iter->second.isValid = false;
}

void EventSystem::pushEvent(SDL_Event e)
{
	SDL_PushEvent(&e);
}

void EventSystem::dispatch(SDL_Event e)
{
	auto& listeners = m_listeners[static_cast<SDL_EventType>(e.type)];
	auto iter = listeners.begin();
	while (iter != listeners.end())
	{
		auto& cb = m_callbacks[*iter];
		if (!cb.isValid)
		{
			iter = listeners.erase(iter);
			continue;
		}

		cb.func(e);
		iter++;
	}
}
