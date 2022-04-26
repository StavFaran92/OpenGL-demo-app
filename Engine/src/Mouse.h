#pragma once
#include "Core.h"
#include "SDL.h"

#include <functional>


class EngineAPI Mouse
{
public:
	enum class MouseButton
	{
		LeftMousebutton,
		RightMousebutton,
		MiddleMousebutton,
	};
	struct MouseState
	{
		int x = 0;
		int y = 0;
		bool lmb = false;
		bool rmb = false;
		bool mmb = false;
	};

	const MouseState& getMouseState();
	void getMousePosition(int& x, int& y);
	bool getButtonPressed(MouseButton button);
	void onMousePressed(MouseButton code, std::function<void(SDL_Event e)> callback) const;
	void onMouseReleased(MouseButton code, std::function<void(SDL_Event e)> callback) const;



private:
	int mouseButtonToSDLCode(MouseButton button) const;
	MouseState m_state;
};
