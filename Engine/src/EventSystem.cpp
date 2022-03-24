#include "EventSystem.h"

void EventSystem::addEventListener(SDL_EventType eventType, std::function<void(SDL_Event e)> callback)
{
	if (m_listeners.find(eventType) == m_listeners.end())
		m_listeners[eventType] = std::vector<std::function<void(SDL_Event e)>>();
	m_listeners[eventType].emplace_back(callback);
}

void EventSystem::removeEventListener(std::function<void(SDL_Event e)> callback)
{

}

void EventSystem::pushEvent(SDL_Event e)
{
	SDL_PushEvent(&e);
}

void EventSystem::dispatch(SDL_Event e)
{
	for (const auto& listener : m_listeners[static_cast<SDL_EventType>(e.type)])
	{
		listener(e);
	}
}
