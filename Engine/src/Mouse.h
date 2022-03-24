#pragma once
#include "Core.h"
#include "SDL.h"


class EngineAPI Mouse
{
public:
	enum class MouseButtons
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
	bool getButtonPressed(MouseButtons button);


private:
	MouseState m_state;
};
