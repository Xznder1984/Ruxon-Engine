#pragma once

#include "ruxon/core/types.h"
#include <memory>

namespace ruxon {
namespace render {

class RHI {
public:
    virtual ~RHI() = default;

    // Creates the graphics context for the given native window
    // (SDL_Window* on SDL3 platforms) and makes it current.
    virtual void Init(void* nativeWindow) = 0;
    virtual void Shutdown() = 0;
    virtual void Clear(f32 r, f32 g, f32 b, f32 a = 1.0f) = 0;
    virtual void SwapBuffers() = 0;

    static std::unique_ptr<RHI> Create();
};

} // namespace render
} // namespace ruxon
