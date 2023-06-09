#pragma once

#include "Quad.h"

template<typename T> class ObjectHandler;

class ScreenQuad
{
public:
	static ObjectHandler<ScreenQuad> GenerateScreenQuad();
};

