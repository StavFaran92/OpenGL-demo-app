#pragma once

#include <memory>
#include <string>
#include <atomic>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include "Core.h"
#include "UUID.h"


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
class Material;
class Texture;
class Cubemap;
class CommonShaders;
class CommonTextures;
class ObjectPicker;
class Assets;
template<typename T>class Resource;
template<typename T>class Factory;
template<typename T>class MemoryPool;

struct InitParams
{
    std::string projectDir;
    bool loadExistingProject = false;
    bool overwriteExisting = false;
    bool templateScene = false;
    bool startSimulationOnStartup = false;
};

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
    CacheSystem* getMemoryManagementSystem() const;
    ObjectManager* getObjectManager() const;
    TimeManager* getTimeManager() const;
    PhysicsSystem* getPhysicsSystem() const;
    RandomNumberGenerator* getRandomSystem() const;
    ShaderLoader* getShaderLoader() const;
    ResourceManager* getResourceManager() const;
    CommonShaders* getCommonShaders() const;
    CommonTextures* getCommonTextures() const;
    template<typename T>MemoryPool<T>* getMemoryPool() const {};
    template<>MemoryPool<Texture>* getMemoryPool() const { return m_memoryPoolTexture.get(); }
    template<>MemoryPool<Mesh>* getMemoryPool() const { return m_memoryPoolMesh.get(); }

    
    
    void loadProject(const std::string& filePath);
    void saveProject();
    std::string getProjectDirectory() const;

    std::shared_ptr<Material> getDefaultMaterial() const;

    void pause();
    void resume();
    void stop();

    // Template function to register a subsystem
    template<typename T> void registerSubSystem(const T* subSystem)
    {
        m_subSystems[typeid(T*)] = (void*)subSystem;
    }

    // Template function to get a registered subsystem
    template<typename T> T* getSubSystem() const
    {
        auto it = m_subSystems.find(typeid(T*));
        if (it != m_subSystems.end())
        {
            return reinterpret_cast<T*>(it->second);
        }
        else
        {
            throw std::exception("Subsystem not found");
        }
    }


    Engine(const Engine&) = delete;
    void operator=(const Engine&) = delete;

protected:
    // -------------------- Methods -------------------- //
    friend void sgeEntry(int, char*[]);
    template<typename T>friend class Factory;

#ifdef TESTBED
    friend class TestEngine;
#endif

    bool init(const InitParams& initParams);
    void update(float deltaTime);
    void draw(float deltaTime);
    void run(Application* app);
    void close();

    void SetContext(std::shared_ptr<Context> context);
    void SetWindow(std::shared_ptr<Window> window);
    void handleEvents(SDL_Event& e, bool& quit);

    void createStartupScene(const std::shared_ptr<Context>& context, const InitParams& initParams);
protected:

    Engine();

    // -------------------- Attributes -------------------- //
    static Engine* instance;

    bool m_isInit = false;

    std::shared_ptr<Window> m_window;
    std::shared_ptr<Context> m_context;
    std::shared_ptr<ImguiHandler> m_imguiHandler;
    std::shared_ptr<Input> m_input;
    std::shared_ptr<EventSystem> m_eventSystem;
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
    std::shared_ptr<CommonShaders> m_commonShaders;
    std::shared_ptr<CommonTextures> m_commonTextures;
    std::shared_ptr<Assets> m_assets;

    std::map<std::type_index, void*> m_subSystems;

    std::shared_ptr<Material> m_defaultMaterial;

    std::atomic<bool> m_isPaused = false;
    std::atomic<bool> m_isStopped = false;

    std::string m_projectDirectory;
};

#define SGE_ROOT_DIR Engine::get()->getRootDir()