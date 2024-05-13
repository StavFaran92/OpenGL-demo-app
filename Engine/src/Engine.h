#pragma once

#include <memory>
#include <string>
#include "Core.h"


class Context;
class IRenderer;
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
class CacheSystem;
class ObjectManager;
class TimeManager;
class PhysicsSystem;
class RandomNumberGenerator;
class ShaderLoader;
class ResourceManager;
class Texture;
class Mesh;
class ProjectManager;
template<typename T>class MemoryPool;

class EngineAPI Engine
{
public:

    // -------------------- Methods -------------------- //
    static Engine* get();

    std::string getRootDir();

    Window* getWindow() const;
    IRenderer* getRenderer() const;
    Context* getContext() const;
    ImguiHandler* getImguiHandler() const;
    Input* getInput() const;
    EventSystem* getEventSystem() const;
    ModelImporter* getModelImporter() const;
    CacheSystem* getMemoryManagementSystem() const;
    ObjectManager* getObjectManager() const;
    TimeManager* getTimeManager() const;
    PhysicsSystem* getPhysicsSystem() const;
    RandomNumberGenerator* getRandomSystem() const;
    ShaderLoader* getShaderLoader() const;
    ResourceManager* getResourceManager() const;
    template<typename T>MemoryPool<T>* getMemoryPool() const {};
    template<>MemoryPool<Texture>* getMemoryPool() const { return m_memoryPoolTexture.get(); }
    template<>MemoryPool<Mesh>* getMemoryPool() const { return m_memoryPoolMesh.get(); }

    
    void loadProject(const std::string& filePath);
    void saveProject(const std::string& filePath);

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
    std::shared_ptr<CacheSystem> m_memoryManagementSystem;
    std::shared_ptr<ObjectManager> m_objectManager;
    std::shared_ptr<TimeManager> m_timeManager;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;
    std::shared_ptr<RandomNumberGenerator> m_randomSystem;
    std::shared_ptr<ShaderLoader> m_shaderLoader;
    std::shared_ptr<ResourceManager> m_resourceManager;
    std::shared_ptr<MemoryPool<Texture>> m_memoryPoolTexture;
    std::shared_ptr<MemoryPool<Mesh>> m_memoryPoolMesh;
    std::shared_ptr<ProjectManager> m_projectManager;

};

#define SGE_ROOT_DIR Engine::get()->getRootDir()