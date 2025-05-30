#pragma once

class VertexBuffer {
private:
    unsigned int m_renderer_id;
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) {
        m_renderer_id = other.m_renderer_id;
        other.m_renderer_id = 0;
    }

    VertexBuffer& operator=(VertexBuffer&& other) {
        if (this != &other) {
            m_renderer_id = other.m_renderer_id;
            other.m_renderer_id = 0;
        }
        return *this;
    }
};
