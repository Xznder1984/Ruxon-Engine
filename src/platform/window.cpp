#include "ruxon/platform/window.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace platform {

class SDLWindow : public Window {
public:
    SDLWindow(const WindowProps& props) : m_Props(props) {
        core::Log::Info("Creating SDL Window: " + props.title);
    }
    
    ~SDLWindow() override {}
    
    void Update() override {
        // Window update logic
    }
    
    u32 GetWidth() const override { return m_Props.width; }
    u32 GetHeight() const override { return m_Props.height; }
    void* GetNativeWindow() const override { return nullptr; }
    
    void SetEventCallback(const EventCallbackFn& callback) override {
        m_Data.EventCallback = callback;
    }
    
    void SetVSync(bool enabled) override {
        m_Props.vsync = enabled;
    }
    
    bool IsVSync() const override { return m_Props.vsync; }
    
private:
    struct WindowData {
        std::string title;
        u32 width, height;
        bool vsync;
        EventCallbackFn EventCallback;
    };
    
    WindowProps m_Props;
    WindowData m_Data;
};

Window* Window::Create(const WindowProps& props) {
    return new SDLWindow(props);
}

} // namespace platform
} // namespace ruxon
