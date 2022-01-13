#pragma once
#include "SDL.h"
#include <GL/glew.h>
#include <cstdint>
#include "Utils/Logger/Logger.h"

class ObjectSelection
{
public:
	ObjectSelection();

	void Init();
	void OnMousePressed(SDL_MouseButtonEvent& e);
	void SelectObject(uint32_t index);
	uint32_t GetSelectedObject();

private:
	uint32_t m_selectedObject = 0;
};

