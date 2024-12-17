#include "index_buffer.h"
#include "renderer.h"

IndexBuffer::IndexBuffer(const unsigned char* data, unsigned int count) : m_count(count) {
    gl_call(glGenBuffers(1, &m_renderer_id));
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    gl_call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned char), data, GL_STATIC_DRAW));
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
