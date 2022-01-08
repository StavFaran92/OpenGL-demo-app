#include "Application.h"

#include "Context.h"
#include "Renderer/Renderer.h"
#include "Window.h"
#include "imgui/ImguiHandler.h"

// Singleton
Application Application::instance;

bool Application::Init()
{
    m_window = std::make_shared<Window>(1024, 768);
    if (!m_window->initialize())
    {
        logError("Window init failed!");
        return false;
    }

    m_context = std::make_shared<Context>();

    m_imguiHandler = std::make_shared<ImguiHandler>();
    if (!m_imguiHandler->Init(m_window->GetWindow(), m_window->GetContext()))
    {
        logError("Imgui init failed!");
        return false;
    }

    return true;
}

std::shared_ptr<Renderer> Application::GetRenderer() { return m_context->GetRenderer(); }

std::shared_ptr<Context> Application::GetContext()
{
    return m_context;
}

void Application::Close()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}
