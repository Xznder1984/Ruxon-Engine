#include "ruxon/render/buffer.h"
#include "ruxon/core/log.h"

namespace ruxon {
namespace render {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(float* vertices, u32 size) {
        core::Log::Info("Creating OpenGL Vertex Buffer");
    }
    void Bind() const override {}
    void Unbind() const override {}
    void SetData(const void* data, u32 size) override {}
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(u32* indices, u32 count) : m_Count(count) {
        core::Log::Info("Creating OpenGL Index Buffer");
    }
    void Bind() const override {}
    void Unbind() const override {}
    u32 GetCount() const override { return m_Count; }
private:
    u32 m_Count;
};

VertexBuffer* VertexBuffer::Create(float* vertices, u32 size) {
    return new OpenGLVertexBuffer(vertices, size);
}

IndexBuffer* IndexBuffer::Create(u32* indices, u32 count) {
    return new OpenGLIndexBuffer(indices, count);
}

} // namespace render
} // namespace ruxon
