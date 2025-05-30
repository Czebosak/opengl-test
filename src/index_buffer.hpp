#pragma once

class IndexBuffer {
private:
    unsigned int m_renderer_id;
    unsigned int m_count;
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int get_count() const { return m_count; }

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) {
        m_renderer_id = other.m_renderer_id;
        m_count = other.m_count;
        other.m_renderer_id = 0;
        other.m_renderer_id = 0;
    }

    IndexBuffer& operator=(IndexBuffer&& other) {
        if (this != &other) {
            m_renderer_id = other.m_renderer_id;
            m_count = other.m_count;
            other.m_renderer_id = 0;
            other.m_count = 0;
        }
        return *this;
    }
};
