#pragma once

#include <memory>
#include "Core.h"


class Context;
class Renderer;
class SkyboxRenderer;
class Window;
class ImguiHandler;
class ObjectSelection;
class ScreenBufferProjector;
class Application;
union SDL_Event;

class EngineAPI Engine
{
public:

    // -------------------- Methods -------------------- //
    static Engine* get();

    Window* getWindow() const;
    Renderer* getRenderer() const;
    Context* getContext() const;
    ImguiHandler* getImguiHandler() const;

    void pause();
    void resume();
    void stop();


    Engine(const Engine&) = delete;
    void operator=(const Engine&) = delete;

private:
    // -------------------- Methods -------------------- //
    friend int main();

    bool init();
    void update(float deltaTime);
    void draw(float deltaTime);
    void run(Application* app);
    void close();

    void SetContext(std::shared_ptr<Context> context);
    void SetWindow(std::shared_ptr<Window> window);
    void handleEvents(SDL_Event& e, bool& quit, double deltaTime);
private:

    Engine();

    // -------------------- Attributes -------------------- //
    static Engine* instance;

    bool m_isInit = false;
    std::shared_ptr<Window> m_window = nullptr;
    std::shared_ptr<Context> m_context = nullptr;
    std::shared_ptr<ImguiHandler> m_imguiHandler = nullptr;



};

