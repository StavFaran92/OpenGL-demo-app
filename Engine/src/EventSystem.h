#pragma once
#include <unordered_map>
#include <vector>
#include <functional>

#include "SDL_events.h"

#include "Core.h"

class EngineAPI EventSystem
{
public:
	void addEventListener(SDL_EventType eventType, const std::function<void(SDL_Event e)>& callback);
	void removeEventListener(std::function<void(SDL_Event e)> callback);
	void pushEvent(SDL_Event e);
private:
	friend class Engine;
	void dispatch(SDL_Event e);
private:
	std::unordered_map<SDL_EventType, std::vector<std::function<void(SDL_Event e)>>> m_listeners;
};
