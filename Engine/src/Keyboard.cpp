#include "Keyboard.h"

#include "Logger.h"

Keyboard::Keyboard() 
{
	m_keyboardState = SDL_GetKeyboardState(&m_length);
}

int Keyboard::getKeyState(SDL_Scancode code) const
{
	if (code < 0 || code > m_length)
	{
		logError("Invalid key specified : " + code);
		return false;
	}

	return m_keyboardState[code];
}