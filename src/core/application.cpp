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

    m_RHI = render::RHI::Create();
    m_RHI->Init(m_Window->GetNativeWindow());

    Log::Info("Application created: " + name);
}

Application::~Application() {
    if (m_RHI) m_RHI->Shutdown();
    m_RHI.reset();
    s_Instance = nullptr;
}

void Application::Run() {
    Log::Info("Application running...");
    while (m_Running) {
        m_Window->Update();
        if (m_Window->ShouldClose()) m_Running = false;

        m_RHI->Clear(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z);
        OnFrame();
        m_RHI->SwapBuffers();
    }
    Log::Info("Application stopped.");
}

void Application::Stop() {
    m_Running = false;
}

} // namespace core
} // namespace ruxon
