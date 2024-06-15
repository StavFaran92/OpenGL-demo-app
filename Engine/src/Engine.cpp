#include "Engine.h"

#include "Context.h"
#include "Renderer.h"
#include "SkyboxRenderer.h"
#include "Window.h"
#include "ImguiHandler.h"
#include "ObjectSelection.h"
#include "PostProcessProjector.h"
#include "ICamera.h"
#include "Scene.h"
#include "Skybox.h"
#include "Input.h"
#include "EventSystem.h"
#include "ModelImporter.h"
#include "Logger.h"
#include "CacheSystem.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "TimeManager.h"
#include "PhysicsSystem.h"
#include "Random.h"
#include "ShadowSystem.h"
#include "ShaderLoader.h"
#include "ShaderParser_tntmeijsImpl.h"
#include "ResourceManager.h"
#include "ProjectManager.h"
#include "ProjectAssetRegistry.h"
#include "ShapeFactory.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "EditorCamera.h"
#include "CommonShaders.h"
#include "CommonTextures.h"

#include "Application.h"
#include "SDL2/SDL.h"

#include "GL/glew.h"

#include <filesystem>

// Singleton
Engine* Engine::instance = nullptr;

bool Engine::init(const InitParams& initParams)
{
    if (m_isInit)
    {
        logError("Engine already started!");
        return false;
    }

    m_projectDirectory = initParams.projectDir;

    if (!std::filesystem::exists(m_projectDirectory) || !std::filesystem::is_directory(m_projectDirectory)) 
    {
        logError("Path does not exist or is not a directory");
        return false;
    }

    // If create new project, check if dir is empty
    if (!initParams.loadExistingProject)
    {
        if (!initParams.overwriteExisting)
        {
            // Iterate over the directory and check if there are any entries
            if (std::filesystem::directory_iterator(m_projectDirectory) != std::filesystem::directory_iterator{})
            {
                logError("Directory is not empty, SGE requires an empty directory to start a new project.");
                return false;
            }
        }
        else
        {
            // Overwrite existing files by deleting all files in the directory
            try {
                for (const auto& entry : std::filesystem::directory_iterator(m_projectDirectory)) {
                    std::filesystem::remove_all(entry); // Remove file or directory
                }
            }
            catch (const std::filesystem::filesystem_error& e) {
                logError("Failed to clear the directory: " + std::string(e.what()));
                return false;
            }
        }

    }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    m_eventSystem = std::make_shared<EventSystem>();

    m_memoryPoolTexture = std::make_shared<MemoryPool<Texture>>();
    m_memoryPoolMesh = std::make_shared<MemoryPool<Mesh>>();

    m_resourceManager = std::make_shared<ResourceManager>();

    m_projectManager = std::make_shared<ProjectManager>();

    if (!SGE_EXPORT_PACKAGE)
    {
        auto found = false;
        try
        {
            size_t len = 0;
            char* sgeRoot = nullptr;
            errno_t err = _dupenv_s(&sgeRoot, &len, "SGE");
            if (err == 0 && sgeRoot)
            {
                found = true;
                m_resourceManager->setRootDir(std::string(sgeRoot) + "/Engine/");
            }
            free(sgeRoot);
        }
        catch (std::exception e)
        {
            logError(e.what());
        }

        if (!found)
        {
            m_resourceManager->setRootDir("./");
        }
        
    }
    else
    {
        m_resourceManager->setRootDir("./");
    }

    m_defaultMaterial = std::make_shared<Material>();

    m_objectManager = std::make_shared<ObjectManager>();

    ObjectFactory::init(m_objectManager.get());

    m_window = std::make_shared<Window>(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!m_window->init())
    {
        logError("Window init failed!");
        return false;
    }

    m_input = std::make_shared<Input>();
    m_input->init();

    auto shaderParser = std::make_shared<ShaderParser_tntmeijs>();

    ShaderLoader::LoadParams lParams;
    lParams.extendShader = true;
    m_shaderLoader = std::make_shared<ShaderLoader>(shaderParser, lParams);

    //auto secondScene = std::make_shared<Scene>();
    //secondScene->setPostProcess(true);
    //m_context->addScene(secondScene);

    m_imguiHandler = std::make_shared<ImguiHandler>();
    if (!m_imguiHandler->init(m_window->GetWindow(), m_window->GetContext()))
    {
        logError("Imgui init failed!");
        return false;
    }

    m_modelImporter = std::make_shared<ModelImporter>();

    m_timeManager = std::make_shared<TimeManager>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();
    if (!m_physicsSystem->init())
    {
        logError("Physics System init failed!");
        return false;
    }

    m_commonShaders = std::make_shared<CommonShaders>();

    m_commonTextures = std::make_shared<CommonTextures>();

    m_randomSystem = std::make_shared<RandomNumberGenerator>();

    if (initParams.loadExistingProject)
    {
        loadProject(m_projectDirectory);
    }
    else
    {
        // Create a new Project
        m_memoryManagementSystem = std::make_shared<CacheSystem>();
        auto& par = ProjectAssetRegistry::create(initParams.projectDir);;
        m_context = std::make_shared<Context>(par);
        
        createStartupScene(m_context, initParams);

        saveProject();
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

std::string Engine::getRootDir()
{
    return m_resourceManager->getRootDir();
}

void Engine::SetWindow(std::shared_ptr<Window> window)
{
    m_window = window;
}

IRenderer* Engine::getRenderer() const
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


void Engine::draw(float deltaTime)
{
    m_context->draw(deltaTime);

    m_imguiHandler->render();

    m_window->SwapBuffer();
}


void Engine::update(float deltaTime)
{
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
        handleEvents(e, quit);

        if (quit)
            return;

        update(deltaTime);
        draw(deltaTime);

    }
}

void Engine::stop()
{
    logTrace(__FUNCTION__);

    SDL_Event e;
    e.type = SDL_QUIT;
    m_eventSystem->pushEvent(e);
}

void Engine::close()
{
    logTrace(__FUNCTION__);

    m_context->close();

    m_imguiHandler->close();

    m_window->close();

    m_isInit = false;

    // TODO fix

    //delete instance;
    //instance = nullptr;
}

ImguiHandler* Engine::getImguiHandler() const
{
    return m_imguiHandler.get();
}

Input* Engine::getInput() const
{
    return m_input.get();
}

EventSystem* Engine::getEventSystem() const
{
    return m_eventSystem.get();
}

ModelImporter* Engine::getModelImporter() const
{
    return m_modelImporter.get();
}

CacheSystem* Engine::getMemoryManagementSystem() const
{
    return m_memoryManagementSystem.get();
}

ObjectManager* Engine::getObjectManager() const
{
    return m_objectManager.get();
}

TimeManager* Engine::getTimeManager() const
{
    return m_timeManager.get();
}

PhysicsSystem* Engine::getPhysicsSystem() const
{
    return m_physicsSystem.get();
}

RandomNumberGenerator* Engine::getRandomSystem() const
{
    return m_randomSystem.get();
}

ShaderLoader* Engine::getShaderLoader() const
{
    return m_shaderLoader.get();
}

ResourceManager* Engine::getResourceManager() const
{
    return m_resourceManager.get();
}

CommonShaders* Engine::getCommonShaders() const
{
    return m_commonShaders.get();
}

CommonTextures* Engine::getCommonTextures() const
{
    return m_commonTextures.get();
}

void Engine::loadProject(const std::string& dirPath)
{
    m_projectDirectory = dirPath;

    

    auto& par = ProjectAssetRegistry::parse(dirPath);
    auto& filePath = par->getFilepath();
    m_context = std::make_shared<Context>(par);

    m_memoryManagementSystem = std::make_shared<CacheSystem>(par->getAssociations());

    m_projectManager->loadProject(filePath, m_context);
}

void Engine::saveProject()
{
    m_projectManager->saveProject();
}

std::string Engine::getProjectDirectory() const
{
    return m_projectDirectory;
}

std::shared_ptr<Material> Engine::getDefaultMaterial() const
{
    return m_defaultMaterial;
}

void Engine::pause()
{
}

void Engine::handleEvents(SDL_Event& e, bool& quit)
{
    while (SDL_PollEvent(&e) != 0)
    {
        m_imguiHandler->proccessEvents(e);

        //User requests quit
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }


        m_eventSystem->dispatch(e);
    }
}

void Engine::createStartupScene(const std::shared_ptr<Context>& context, const InitParams& initParams)
{
    auto startupScene = std::make_shared<Scene>(m_context.get());

    m_context->addScene(startupScene);
    m_context->setActiveScene(startupScene->getID());

    // Add default dir light
    auto dLight = startupScene->createEntity("Directional light");
    dLight.addComponent<DirectionalLight>();
    dLight.getComponent<Transformation>().setLocalRotation(glm::vec3(0, -1, 0));

    auto editorCamera = startupScene->createEntity("Editor Camera");
    editorCamera.addComponent<CameraComponent>();
    editorCamera.addComponent<NativeScriptComponent>().bind<EditorCamera>();
    editorCamera.RemoveComponent<ObjectComponent>();
    startupScene->setPrimaryEditorCamera(editorCamera);

    auto mainCamera = startupScene->createEntity("Main Camera");
    mainCamera.addComponent<CameraComponent>();
    startupScene->setPrimarySceneCamera(mainCamera);
    mainCamera.getComponent<Transformation>().setLocalPosition({10,1,10});
    mainCamera.getComponent<CameraComponent>().center = {0,0,0};
    mainCamera.getComponent<CameraComponent>().up = {0,1,0};

    if (initParams.templateScene)
    {
        Skybox::CreateSkyboxFromCubemap({ SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/right.jpg",
            SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/left.jpg",
            SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/top.jpg",
            SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/bottom.jpg",
            SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/front.jpg",
            SGE_ROOT_DIR + "Resources/Engine/Textures/Skybox/back.jpg" }, context->getActiveScene().get());


        {
            auto ground = ShapeFactory::createBox(&context->getActiveScene()->getRegistry());
            auto& groundTransfrom = ground.getComponent<Transformation>();
            groundTransfrom.setLocalScale({ 50, .5f, 50 });
            auto& mat = ground.addComponent<MaterialComponent>();
            auto tex = Texture::create2DTextureFromFile(SGE_ROOT_DIR + "Resources/Engine/Textures/floor.jpg");
            mat.begin()->get()->setTexture(Texture::Type::Albedo, tex);
            auto& rb = ground.addComponent<RigidBodyComponent>(RigidbodyType::Static, 1.f);
            auto& collisionBox = ground.addComponent<CollisionBoxComponent>(.5f);
        }

        {
            //editorCamera->lookAt(0, 5, 0);
            //editorCamera->setPosition(25, 225, 35);
        }

        auto sphere = ShapeFactory::createSphere(&context->getActiveScene()->getRegistry());
        {
            auto random = Engine::get()->getRandomSystem();
            auto x = random->rand() * 10 - 5;
            auto z = random->rand() * 10 - 5;

            auto& sphereTransform = sphere.getComponent<Transformation>();
            sphereTransform.setLocalPosition({ x, 10, z });


            auto& mat = sphere.addComponent<MaterialComponent>();
            auto tex = Texture::create2DTextureFromFile(SGE_ROOT_DIR + "Resources/Engine/Textures/floor.jpg");
            mat.begin()->get()->setTexture(Texture::Type::Diffuse, tex);

            auto& rb = sphere.addComponent<RigidBodyComponent>(RigidbodyType::Dynamic, 1.f);
            auto& collisionBox = sphere.addComponent<CollisionSphereComponent>(1.f);
        }
    }
}

void Engine::resume()
{
}
