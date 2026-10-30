// SDL3-backed window and event loop (platform layer, section 5.1).
// SDL owns NSApp on macOS so the process stays responsive to the system.

#include "ruxon/platform/window.h"
#include "ruxon/core/log.h"
#include <SDL3/SDL.h>

namespace ruxon {
namespace platform {

class SDLWindow : public Window {
public:
    explicit SDLWindow(const WindowProps& props) : m_Props(props) {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            core::Log::Error(std::string("SDL_Init failed: ") + SDL_GetError());
            return;
        }


        SDL_WindowFlags flags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL;
        if (props.resizable) flags |= SDL_WINDOW_RESIZABLE;
        if (props.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

        // GL 4.1 core: the newest profile macOS supports; widely available
        // (and forward-compatible with newer drivers) on Windows/Linux.
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_Window = SDL_CreateWindow(props.title.c_str(),
                                    static_cast<int>(props.width),
                                    static_cast<int>(props.height),
                                    flags);
        if (!m_Window) {
            core::Log::Error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
            return;
        }

        core::Log::Info("Window opened: " + props.title);
    }

    ~SDLWindow() override {
        if (m_Window) SDL_DestroyWindow(m_Window);
        if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    void Update() override {
        if (!m_Window) return;
        SDL_Event event;
        // Idle-wait (no busy spin) for events or ~frame time, then pump.
        if (SDL_WaitEventTimeout(&event, 16)) {
            do {
                HandleEvent(event);
            } while (SDL_PollEvent(&event));
        } else {
            SDL_PumpEvents();
        }
    }

    u32 GetWidth() const override {
        int w = 0, h = 0;
        if (m_Window) SDL_GetWindowSize(m_Window, &w, &h);
        return static_cast<u32>(w);
    }

    u32 GetHeight() const override {
        int w = 0, h = 0;
        if (m_Window) SDL_GetWindowSize(m_Window, &w, &h);
        return static_cast<u32>(h);
    }

    void* GetNativeWindow() const override {
        return static_cast<void*>(m_Window);
    }

    bool ShouldClose() const override { return m_ShouldClose.load(); }

    void SetVSync(bool enabled) override { m_Props.vsync = enabled; }
    bool IsVSync() const override { return m_Props.vsync; }

private:
    void HandleEvent(const SDL_Event& event) {
        if (m_EventCallback) m_EventCallback(const_cast<SDL_Event*>(&event));
        switch (event.type) {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                m_ShouldClose.store(true);
                break;
            default:
                break;
        }
    }



    WindowProps m_Props;
    SDL_Window* m_Window = nullptr;
    std::atomic<bool> m_ShouldClose{false};
};

Window* Window::Create(const WindowProps& props) {
    return new SDLWindow(props);
}

} // namespace platform
} // namespace ruxon
