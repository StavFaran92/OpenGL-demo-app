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
#include "Skybox.h"

#include "Application.h"
#include "SDL.h"

// Singleton
Engine* Engine::instance = nullptr;

bool Engine::init()
{
    if (m_isInit)
    {
        logError("Engine already started!");
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    m_window = std::make_shared<Window>(1024, 768);
    if (!m_window->init())
    {
        logError("Window init failed!");
        return false;
    }

    m_context = std::make_shared<Context>();

    auto defaultScene = std::make_shared<Scene>();
    defaultScene->setPostProcess(true);

    auto skybox = std::shared_ptr<Skybox>(Skybox::CreateSkybox());
    defaultScene->setSkybox(skybox);

    m_context->addScene(defaultScene);
    m_context->setActiveScene(defaultScene->getID());

    auto secondScene = std::make_shared<Scene>();
    secondScene->setPostProcess(true);
    m_context->addScene(secondScene);

    m_imguiHandler = std::make_shared<ImguiHandler>();
    if (!m_imguiHandler->init(m_window->GetWindow(), m_window->GetContext()))
    {
        logError("Imgui init failed!");
        return false;
    }

    m_isInit = true;

    logInfo("SGE Initialized Successfully!");

    return true;
}

Window* Engine::getWindow() const
{
    return m_window.get();
}

Engine::Engine()
{}

Engine* Engine::get()
{
    if (instance == nullptr)
    {
        instance = new Engine();
    }
    return instance;
}

void Engine::SetWindow(std::shared_ptr<Window> window)
{
    m_window = window;
}

Renderer* Engine::getRenderer() const
{ 
    return m_context->getActiveScene()->getRenderer().get(); 
}

void Engine::SetContext(std::shared_ptr<Context> context)
{
    m_context = context; 
}

Context* Engine::getContext() const
{
    return m_context.get();
}


void Engine::draw()
{


    m_context->draw();

    m_imguiHandler->Render();

    m_window->SwapBuffer();
}


void Engine::update(float deltaTime)
{
    //lightCube->GetTransformation()->SetPosition({ 10 * cos(angle * Constants::toRadians) ,0, 10 * sin(angle * Constants::toRadians) });
    //angle++;
    //lightCube->GetTransformation()->SetScale({ .25f, .25f, .25f });

    m_context->update(deltaTime);
}

void Engine::run(Application* app)
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

        update(deltaTime);
        app->draw();
        draw();

    }
}

void Engine::stop()
{
    logTrace(__FUNCTION__);

    SDL_Event e;
    e.type = SDL_QUIT;
    SDL_PushEvent(&e);
}

void Engine::close()
{
    logTrace(__FUNCTION__);

    m_imguiHandler->close();

    m_window->close();

    m_isInit = false;

    delete instance;
    instance = nullptr;
}

ImguiHandler* Engine::getImguiHandler() const
{
    return m_imguiHandler.get(); 
}

void Engine::pause()
{
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

void Engine::resume()
{
}
