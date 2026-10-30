#pragma once

#include "ruxon/core/types.h"
#include <string>
#include <functional>
#include <atomic>

namespace ruxon {
namespace platform {

struct WindowProps {
    std::string title = "Ruxon Engine";
    u32 width = 1280;
    u32 height = 720;
    bool fullscreen = false;
    bool resizable = true;
    bool vsync = true;
};

class Window {
public:
    using EventCallbackFn = std::function<void(void*)>;

    virtual ~Window() = default;

    // Pumps and dispatches OS events. Must be called once per frame.
    virtual void Update() = 0;
    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;
    virtual void* GetNativeWindow() const = 0;

    // True once the user closed the window; the application loop checks this.
    virtual bool ShouldClose() const = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    // Receives pumped OS events (typed as SDL_Event* here; kept void* so the
    // platform layer stays SDL-free in its public interface). Install before
    // Run, e.g. to forward input to the editor's UI library.
    void SetEventCallback(EventCallbackFn cb) { m_EventCallback = std::move(cb); }

    static Window* Create(const WindowProps& props = WindowProps());

protected:
    EventCallbackFn m_EventCallback;
};

} // namespace platform
} // namespace ruxon
