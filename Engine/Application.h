#pragma once

class Application
{
public:
	virtual void start()=0;
	virtual void update()=0;

	Application() {};
	virtual ~Application() { };
};
