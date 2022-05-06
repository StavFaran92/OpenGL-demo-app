#pragma once

#include "SDL.h"
#include "Core.h"

#include <deque>

class GuiMenu;

class EngineAPI ImguiHandler
{
public:
	bool init(SDL_Window* window, const SDL_GLContext& context);
	void proccessEvents(SDL_Event& e);
	void render();
	bool close();

	void draw(GuiMenu* menu);

private:
	std::deque<GuiMenu*> m_guiQueue;
};

