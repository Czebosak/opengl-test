#include "vertex_array.hpp"

#include <vertex_buffer_layout.hpp>
#include <renderer.hpp>

VertexArray::VertexArray() {
    gl_call(glGenVertexArrays(1, &id));
}

VertexArray::~VertexArray() {
    gl_call(glDeleteVertexArrays(1, &id));
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();
    const auto& elements = layout.get_elements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        gl_call(glEnableVertexAttribArray(i));
        gl_call(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.get_stride(), (const void*)offset));
        offset += element.count * VertexBufferElement::get_size_of_type(element.type);
    }
}

void VertexArray::bind() const {
    gl_call(glBindVertexArray(id));
}

void VertexArray::unbind() const {
    gl_call(glBindVertexArray(0));
}
