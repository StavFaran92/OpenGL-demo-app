#include "Keyboard.h"

Keyboard::Keyboard() {
	for (size_t i = 0; i < 1024; i++)
	{
		m_keys[i] = 0;
	}
}

bool Keyboard::getKeyPressed(int index)
{
	if (index < 0 || index > NUM_OF_KEYS)
	{
		std::cout << "Invalid key specified : " + index << std::endl;
		return false; //TODO fix
	}

	return m_keys[index];
}
