#include "vertex_buffer.hpp"
#include "renderer.hpp"

VertexBuffer::VertexBuffer() {
    id = 0;
}

VertexBuffer::VertexBuffer(const void* data, size_t size) {
    gl_call(glGenBuffers(1, &id));
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, id));
    gl_call(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
    gl_call(glDeleteBuffers(1, &id));
}

void VertexBuffer::bind() const {
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void VertexBuffer::unbind() const {
    gl_call(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::set_data(const void* data, size_t size) {
    gl_call(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

void VertexBuffer::resize(size_t new_size) {
    gl_call(glBufferData(GL_ARRAY_BUFFER, new_size, nullptr, GL_DYNAMIC_DRAW));
}
