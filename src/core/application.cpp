#include "ruxon/core/application.h"
#include "ruxon/core/assert.h"

namespace ruxon {
namespace core {

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name) {
    RUXON_ASSERT(!s_Instance);
    s_Instance = this;
    
    platform::WindowProps props;
    props.title = name;
    m_Window = std::unique_ptr<platform::Window>(platform::Window::Create(props));
    
    Log::Info("Application created: " + name);
}

Application::~Application() {
    s_Instance = nullptr;
}

void Application::Run() {
    Log::Info("Application running...");
    while (m_Running) {
        m_Window->Update();
        if (m_Window->ShouldClose()) m_Running = false;
    }
    Log::Info("Application stopped.");
}

void Application::Stop() {
    m_Running = false;
}

} // namespace core
} // namespace ruxon
