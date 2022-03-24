#pragma once

#include <iostream>
#include "Core.h"

#include "SDL.h"

class EngineAPI Keyboard
{
public:
	Keyboard();
	int getKeyState(SDL_Scancode index) const;

private:
	const Uint8* m_keyboardState;
	int m_length;
};
