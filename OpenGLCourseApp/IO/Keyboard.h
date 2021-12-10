#pragma once

#include <iostream>

static const int NUM_OF_KEYS = 1024;

class Keyboard
{
public:
	Keyboard();
	bool getKeyPressed(int index);

private:
	bool m_keys[NUM_OF_KEYS];
};

