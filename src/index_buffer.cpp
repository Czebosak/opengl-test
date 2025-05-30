#include "index_buffer.hpp"
#include "renderer.hpp"

IndexBuffer::IndexBuffer() {}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_count(count) {
    gl_call(glGenBuffers(1, &m_renderer_id));
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    gl_call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    gl_call(glDeleteBuffers(1, &m_renderer_id));
}

void IndexBuffer::bind() const {
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void IndexBuffer::unbind() const {
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
