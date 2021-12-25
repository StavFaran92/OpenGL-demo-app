#include "Keyboard.h"

Keyboard::Keyboard() {
	m_keyboardState = SDL_GetKeyboardState(&m_length);
}

int Keyboard::getKeyState(int index)
{
	if (index < 0 || index > m_length)
	{
		std::cout << "Invalid key specified : " + index << std::endl;
		return false;
	}

	return m_keyboardState[index];
}