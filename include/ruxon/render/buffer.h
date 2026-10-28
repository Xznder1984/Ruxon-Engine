#pragma once

#include "ruxon/core/types.h"
#include <vector>

namespace ruxon {
namespace render {

enum class BufferUsage {
    Static,
    Dynamic,
    Stream
};

class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SetData(const void* data, u32 size) = 0;
    
    static VertexBuffer* Create(float* vertices, u32 size);
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual u32 GetCount() const = 0;
    
    static IndexBuffer* Create(u32* indices, u32 count);
};

} // namespace render
} // namespace ruxon
