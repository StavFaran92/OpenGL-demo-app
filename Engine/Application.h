#pragma once

#include "Core.h"

class EngineAPI Application
{
public:
	virtual void start()=0;
	virtual void update()=0;

	Application() = default;
	~Application() = default;

};

Application* CreateApplication();