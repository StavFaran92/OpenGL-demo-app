#pragma once

#include <memory>
#include "Core.h"


class Context;
class Renderer;
class SkyboxRenderer;
class Window;
class ImguiHandler;
class ObjectSelection;
class PostProcessProjector;
class Application;
union SDL_Event;
class Input;
class EventSystem;
class ModelImporter;
class MemoryManagement;
class ObjectManager;
class TimeManager;
class PhysicsSystem;

class EngineAPI Engine
{
public:

    // -------------------- Methods -------------------- //
    static Engine* get();

    Window* getWindow() const;
    Renderer* getRenderer() const;
    Context* getContext() const;
    ImguiHandler* getImguiHandler() const;
    Input* getInput() const;
    EventSystem* getEventSystem() const;
    ModelImporter* getModelImporter() const;
    MemoryManagement* getMemoryManagementSystem() const;
    ObjectManager* getObjectManager() const;
    TimeManager* getTimeManager() const;
    PhysicsSystem* getPhysicsSystem() const;

    void pause();
    void resume();
    void stop();


    Engine(const Engine&) = delete;
    void operator=(const Engine&) = delete;

protected:
    // -------------------- Methods -------------------- //
    friend void sgeEntry();

#ifdef TESTBED
    friend class TestEngine;
#endif

    bool init();
    void update(float deltaTime);
    void draw(float deltaTime);
    void run(Application* app);
    void close();

    void SetContext(std::shared_ptr<Context> context);
    void SetWindow(std::shared_ptr<Window> window);
    void handleEvents(SDL_Event& e, bool& quit);
protected:

    Engine();

    // -------------------- Attributes -------------------- //
    static Engine* instance;

    bool m_isInit = false;
    std::shared_ptr<Window> m_window = nullptr;
    std::shared_ptr<Context> m_context = nullptr;
    std::shared_ptr<ImguiHandler> m_imguiHandler = nullptr;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<EventSystem> m_eventSystem;
    std::shared_ptr<ModelImporter> m_modelImporter;
    std::shared_ptr<MemoryManagement> m_memoryManagementSystem;
    std::shared_ptr<ObjectManager> m_objectManager;
    std::shared_ptr<TimeManager> m_timeManager;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;


};

