#include "Engine.h"

#include "Context.h"
#include "Renderer.h"
#include "SkyboxRenderer.h"
#include "Window.h"
#include "ImguiHandler.h"
#include "ObjectSelection.h"
#include "ScreenBufferProjector.h"
#include "ICamera.h"
#include "Scene.h"

#include "Application.h"
#include "SDL.h"

// Singleton
Engine* Engine::instance = nullptr;

Renderer* Engine::defaultRenderer = nullptr;
Renderer* Engine::skyboxRenderer = nullptr;

bool Engine::Init()
{
    if (m_isInit)
    {
        logError("Engine already started!");
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    m_window = std::make_shared<Window>(1024, 768);
    if (!m_window->Init())
    {
        logError("Window init failed!");
        return false;
    }

    defaultRenderer = new Renderer();
    skyboxRenderer = new SkyboxRenderer(*defaultRenderer);

    m_context = std::make_shared<Context>();

    auto defaultScene = std::make_shared<Scene>();
    m_context->addScene(defaultScene);
    m_context->setActiveScene(defaultScene->getID());

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

    logInfo("SGE Initialized Successfully!");

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

std::shared_ptr<Renderer> Engine::GetRenderer() { return m_context->getActiveScene()->getRenderer(); }

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

void Engine::Update(float deltaTime)
{
    //lightCube->GetTransformation()->SetPosition({ 10 * cos(angle * Constants::toRadians) ,0, 10 * sin(angle * Constants::toRadians) });
    //angle++;
    //lightCube->GetTransformation()->SetScale({ .25f, .25f, .25f });

    m_context->update(deltaTime);

    m_screenBufferProjector->RedirectToFrameBuffer();

    m_context->draw();

    m_screenBufferProjector->RedirectToDefault();

    m_imguiHandler->Render();

    m_window->SwapBuffer();
}

void Engine::Run(Application* app)
{
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    // Loop until window closed
    while (!quit)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)SDL_GetPerformanceFrequency() / ((NOW - LAST) * 1000);

        //Handle events on queue
        handleEvents(e, quit, deltaTime);

        if (quit)
            return;

        app->update();
        Update(deltaTime);

    }
}

void Engine::Stop()
{
    logTrace(__FUNCTION__);

    SDL_Event e;
    e.type = SDL_QUIT;
    SDL_PushEvent(&e);
}

void Engine::Close()
{
    logTrace(__FUNCTION__);

    m_imguiHandler->Close();

    m_window->Close();

    delete defaultRenderer;
    delete skyboxRenderer;

    m_isInit = false;

    delete instance;
    instance = nullptr;
}

void Engine::handleEvents(SDL_Event& e, bool& quit, double deltaTime)
{
    while (SDL_PollEvent(&e) != 0)
    {
        m_imguiHandler->ProccessEvents(e);

        //User requests quit
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
        //Handle keypress with current mouse position
        else if (e.type == SDL_MOUSEMOTION)
        {
            m_context->getActiveScene()->getRenderer()->GetCamera()->OnMouseMotion(e.motion.xrel, e.motion.yrel);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            m_context->getActiveScene()->getRenderer()->GetCamera()->OnMousePressed(e.button);
        }
        else if (e.type == SDL_MOUSEBUTTONUP)
        {
            m_context->getActiveScene()->getRenderer()->GetCamera()->OnMouseReleased(e.button);
        }
        else if (e.type == SDL_MOUSEWHEEL)
        {
            m_context->getActiveScene()->getRenderer()->GetCamera()->OnMouseScroll(e.wheel.y);
        }
    }
}