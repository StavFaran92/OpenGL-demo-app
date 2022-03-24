#pragma once
#include "Core.h"

class Shader;

class EngineAPI PostProcess
{
public:
	static Shader* inversion();
	static Shader* grayscale();
};
