#pragma once

#include "ruxon/platform/window.h"
#include "ruxon/render/rhi.h"
#include "ruxon/math/math.h"
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
    render::RHI& GetRHI() { return *m_RHI; }

    void SetClearColor(f32 r, f32 g, f32 b) { m_ClearColor = {r, g, b}; }

private:
    std::unique_ptr<platform::Window> m_Window;
    std::unique_ptr<render::RHI> m_RHI;
    math::vec3 m_ClearColor{0.086f, 0.094f, 0.106f}; // editor dark theme windowBg
    bool m_Running = true;
    static Application* s_Instance;
};

Application* CreateApplication();

} // namespace core
} // namespace ruxon
