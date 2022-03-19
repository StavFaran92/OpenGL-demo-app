#pragma once
#include "Core.h"

class Model;

class EngineAPI Application
{
public:
	virtual void start()=0;
	virtual void draw()=0;
	virtual void close() {};
	
protected:
	/** API */
	void draw(Model* model);

	Application() = default;
	~Application() = default;

private:


};

Application* CreateApplication();