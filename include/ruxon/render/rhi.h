#pragma once

#include "ruxon/core/types.h"
#include <memory>

namespace ruxon {
namespace render {

class RHI {
public:
    virtual ~RHI() = default;
    
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void Clear() = 0;
    virtual void SwapBuffers() = 0;
    
    static std::unique_ptr<RHI> Create();
};

} // namespace render
} // namespace ruxon
