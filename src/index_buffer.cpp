#include "index_buffer.hpp"
#include "renderer.hpp"

IndexBuffer::IndexBuffer() {}

IndexBuffer::IndexBuffer(const u32* data, u32 count) : count(count) {
    gl_call(glGenBuffers(1, &id));
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    gl_call(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
    gl_call(glDeleteBuffers(1, &id));
}

void IndexBuffer::bind() const {
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::unbind() const {
    gl_call(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
