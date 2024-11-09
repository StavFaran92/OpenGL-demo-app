#pragma once

#include "Core.h"

class EngineAPI System
{
public:
	System();

	double getFPS() const;

	void setFPS(double fps);

private:
	double m_fps = 0;
};