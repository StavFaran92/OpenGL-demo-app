#pragma once

#include <memory>

class Context;
class Renderer;
class Window;
class ImguiHandler;
class ObjectSelection;
class ScreenBufferProjector;

class Engine
{
public:
    
    // -------------------- Methods -------------------- //
    static Engine* Get();

    bool Init();
    std::shared_ptr<Window> GetWindow() { return m_window; }
    void SetWindow(std::shared_ptr<Window> window) { m_window = std::shared_ptr<Window>(window); }
    std::shared_ptr<Renderer> GetRenderer();
    std::shared_ptr<Context> GetContext();
    void SetConext(std::shared_ptr<Context> context) {m_context = std::shared_ptr<Context>(context); }

    std::shared_ptr<ImguiHandler> GetImguiHandler() { return m_imguiHandler; }
    std::shared_ptr<ObjectSelection> GetObjectSelection();
    std::shared_ptr<ScreenBufferProjector> GetScreenBufferProjector();

    void Close();

    Engine(const Engine&) = delete;
    void operator=(const Engine&) = delete;

private:

    Engine();

    // -------------------- Attributes -------------------- //
    static Engine* instance;

    bool m_isInit = false;
    std::shared_ptr<Window> m_window = nullptr;
    std::shared_ptr<Context> m_context = nullptr;
    
    std::shared_ptr<ImguiHandler> m_imguiHandler = nullptr;
    std::shared_ptr<ObjectSelection> m_objectSelection = nullptr;
    std::shared_ptr<ScreenBufferProjector> m_screenBufferProjector = nullptr;


};