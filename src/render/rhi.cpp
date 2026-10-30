// OpenGL RHI: the real first stage of section 6.1. Creates a GL context on the
// SDL window, clears to the requested color each frame, and swaps buffers.
// Only GL 1.x commands are used for now (clear/swap); the loader arrives with
// the real draw pipeline, so no third-party GL loader is vendored yet.

#include "ruxon/render/rhi.h"
#include "ruxon/core/log.h"

#ifdef __APPLE__
#define RUXON_GL_HEADER <OpenGL/gl3.h>
#else
#define RUXON_GL_HEADER <GL/gl.h>
#endif
#include RUXON_GL_HEADER

#include <SDL3/SDL.h>
#include <string>

namespace ruxon {
namespace render {

class OpenGLRHI : public RHI {
public:
    void Init(void* nativeWindow) override;
    void Shutdown() override;
    void Clear(f32 r, f32 g, f32 b, f32 a) override;
    void SwapBuffers() override;

private:
    SDL_Window* m_Window = nullptr;
    SDL_GLContext m_Context = nullptr;
};

void OpenGLRHI::Init(void* nativeWindow) {
    core::Log::Info("Initializing OpenGL RHI");

    if (!nativeWindow) {
        core::Log::Error("OpenGL RHI: no native window provided");
        return;
    }

    m_Window = static_cast<SDL_Window*>(nativeWindow);

    SDL_GL_SetSwapInterval(1); // vsync on by default
    m_Context = SDL_GL_CreateContext(m_Window);
    if (!m_Context) {
        core::Log::Error(std::string("SDL_GL_CreateContext failed: ") + SDL_GetError());
        m_Window = nullptr;
        return;
    }

    if (!SDL_GL_MakeCurrent(m_Window, m_Context)) {
        core::Log::Error(std::string("SDL_GL_MakeCurrent failed: ") + SDL_GetError());
        return;
    }

    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    if (version && renderer) {
        core::Log::Info(std::string("OpenGL ") + reinterpret_cast<const char*>(version) +
                        " on " + reinterpret_cast<const char*>(renderer));
    }
}

void OpenGLRHI::Shutdown() {
    if (m_Context) {
        SDL_GL_DestroyContext(m_Context);
        m_Context = nullptr;
    }
    m_Window = nullptr;
    core::Log::Info("Shutting down OpenGL RHI");
}

void OpenGLRHI::Clear(f32 r, f32 g, f32 b, f32 a) {
    if (!m_Context) return;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRHI::SwapBuffers() {
    if (m_Window && m_Context) SDL_GL_SwapWindow(m_Window);
}

std::unique_ptr<RHI> RHI::Create() {
    return std::make_unique<OpenGLRHI>();
}

} // namespace render
} // namespace ruxon
