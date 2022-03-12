#include "Engine.h"

#include "Context.h"
#include "Graphics/Renderer/Renderer.h"
#include "Window.h"
#include "imgui/ImguiHandler.h"
#include "Services/ObjectSelection.h"
#include "Graphics/Services/ScreenBufferProjector.h"

// Singleton
Engine* Engine::instance = nullptr;

bool Engine::Init()
{
    if (m_isInit)
    {
        logError("Engine already started!");
        return false;
    }

    m_window = std::make_shared<Window>(1024, 768);
    if (!m_window->Init())
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

    m_objectSelection = std::make_shared<ObjectSelection>();
    m_objectSelection->Init();

    m_screenBufferProjector = std::make_shared<ScreenBufferProjector>();
    if(!m_screenBufferProjector->Init())
    {
        logError("Screen buffer projector failed to init!");
        return false;
    }

    m_isInit = true;

    return true;
}

Engine::Engine()
{}

Engine* Engine::Get()
{
    if (instance == nullptr)
    {
        instance = new Engine();
    }
    return instance;
}

std::shared_ptr<Renderer> Engine::GetRenderer() { return m_context->GetRenderer(); }

std::shared_ptr<Context> Engine::GetContext()
{
    return m_context;
}

std::shared_ptr<ObjectSelection> Engine::GetObjectSelection()
{
    return m_objectSelection; 
}

std::shared_ptr<ScreenBufferProjector> Engine::GetScreenBufferProjector()
{
    return m_screenBufferProjector;
}

void Engine::Close()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);

    m_isInit = false;

    delete instance;
    instance = nullptr;
}
