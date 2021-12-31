#pragma once

#include "Window.h"
#include "Context.h"

class Application
{
public:
    static Application& Get()
    {
        return instance;
    }

    bool Init();

    std::shared_ptr<Window> GetWindow() { return m_window; }
    void SetWindow(std::shared_ptr<Window> window) { m_window = window; }

    std::shared_ptr<Context> GetContext() { return m_context; }
    void SetConext(std::shared_ptr<Context> context) {
        m_context = context;
    }

    void Close();

    Application(Application const&) = delete;
    void operator=(Application const&) = delete;

private:
    Application() { }

    static Application instance;
    std::shared_ptr<Window> m_window = nullptr;
    std::shared_ptr<Context> m_context = nullptr;

};
