#include "Keyboard.h"

Keyboard::Keyboard() {
	m_keyboardState = SDL_GetKeyboardState(&m_length);
}

int Keyboard::getKeyState(int index)
{
	if (index < 0 || index > m_length)
	{
		logError("Invalid key specified : " + index);
		return false;
	}

	return m_keyboardState[index];
}