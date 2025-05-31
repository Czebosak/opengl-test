#include "vertex_buffer.hpp"
#include "renderer.hpp"

VertexBuffer::VertexBuffer() {
    id = 0;
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
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
