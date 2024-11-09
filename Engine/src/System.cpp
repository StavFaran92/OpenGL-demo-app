#include "System.h"

#include "Engine.h"

System::System()
{
	Engine::get()->registerSubSystem<System>(this);
}

double System::getFPS() const
{
	return m_fps;
}

void System::setFPS(double fps)
{
	m_fps = fps;
}
