#pragma once

#include "ruxon/core/types.h"
#include <string>
#include <functional>

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
    using EventCallbackFn = std::function<void()>;

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

    static Window* Create(const WindowProps& props = WindowProps());
};

} // namespace platform
} // namespace ruxon
