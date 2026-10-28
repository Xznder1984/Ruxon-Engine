#pragma once

#include "ruxon/platform/window.h"
#include "ruxon/core/log.h"
#include <memory>

namespace ruxon {
namespace core {

class Application {
public:
    Application(const std::string& name = "Ruxon App");
    virtual ~Application();
    
    void Run();
    void Stop();
    
    static Application& Get() { return *s_Instance; }
    
    platform::Window& GetWindow() { return *m_Window; }
    
private:
    std::unique_ptr<platform::Window> m_Window;
    bool m_Running = true;
    static Application* s_Instance;
};

Application* CreateApplication();

} // namespace core
} // namespace ruxon
