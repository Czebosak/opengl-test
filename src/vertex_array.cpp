#include "vertex_array.hpp"

#include <vertex_buffer_layout.hpp>
#include <renderer.hpp>

VertexArray::VertexArray() {
    gl_call(glGenVertexArrays(1, &id));
    attribute_index = 0;
}

VertexArray::~VertexArray() {
    gl_call(glDeleteVertexArrays(1, &id));
}

void VertexArray::add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();
    const auto& elements = layout.get_elements();
    u32 offset = 0;
    int i = 0;
    for (i; i < elements.size(); i++) {
        if (i + attribute_index == 2) { std::cout << "HIIIII\n"; }
        const auto& element = elements[i];
        gl_call(glEnableVertexAttribArray(i + attribute_index));

        if (element.type == GL_INT || element.type == GL_UNSIGNED_INT) {
            gl_call(glVertexAttribIPointer(i + attribute_index, element.count, element.type, layout.get_stride(), (const void*)(uintptr_t)offset));
        } else {
            gl_call(glVertexAttribPointer(i + attribute_index, element.count, element.type, element.normalized, layout.get_stride(), (const void*)(uintptr_t)offset));
        }
        
        gl_call(glVertexAttribDivisor(i + attribute_index, layout.get_divisor()));
        offset += element.count * VertexBufferElement::get_size_of_type(element.type);
    }
    attribute_index += i;
}

void VertexArray::bind() const {
    gl_call(glBindVertexArray(id));
}

void VertexArray::unbind() const {
    gl_call(glBindVertexArray(0));
}
