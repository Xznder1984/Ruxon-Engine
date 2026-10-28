#include "ruxon/render/rhi.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace render {

class OpenGLRHI : public RHI {
public:
    void Init() override { core::Log::Info("Initializing OpenGL RHI"); }
    void Shutdown() override { core::Log::Info("Shutting down OpenGL RHI"); }
    void Clear() override {}
    void SwapBuffers() override {}
};

std::unique_ptr<RHI> RHI::Create() {
    return std::make_unique<OpenGLRHI>();
}

} // namespace render
} // namespace ruxon
