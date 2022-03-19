#pragma once

#include <iostream>

#include "Logger.h"

#include "SDL.h"

class Keyboard
{
public:
	Keyboard();
	int getKeyState(int index);

private:
	const Uint8* m_keyboardState;
	int m_length;
};
