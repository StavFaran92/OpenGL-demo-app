#include "Application.h"

// Singleton
Application Application::instance;

bool Application::Init()
{
    m_context = std::make_shared<Context>();

    m_window = std::make_shared<Window>(1024, 768);
    if (!m_window->initialize())
    {
        logError("Window init failed!");
        return false;
    }
}

void Application::Close()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}
