#include "Mouse.h"

const Mouse::MouseState& Mouse::getMouseState()
{
	int x, y;
	Uint32 buttons;

	SDL_PumpEvents();  // make sure we have the latest mouse state.

	buttons = SDL_GetMouseState(&x, &y);

	m_state.x = x;
	m_state.y = y;
	m_state.lmb = buttons & SDL_BUTTON_LMASK;
	m_state.rmb = buttons & SDL_BUTTON_RMASK;
	m_state.mmb = buttons & SDL_BUTTON_MMASK;

	return m_state;
}

void Mouse::getMousePosition(int& x, int& y)
{
	const MouseState& state = getMouseState();

	x = state.x;
	y = state.y;
}

bool Mouse::getButtonPressed(MouseButtons button) 
{
	bool result = false;

	const MouseState& state = getMouseState();

	switch (button)
	{
	case MouseButtons::LeftMousebutton:
		result = state.lmb;
		break;
	case MouseButtons::RightMousebutton:
		result = state.rmb;
		break;
	case MouseButtons::MiddleMousebutton:
		result = state.mmb;
		break;
	default:
		break;
	}
	
	return result;
}
