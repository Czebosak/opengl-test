#pragma once
#include <GL/glew.h>

#include "utils.hpp"
#include <assert.h>
#include <vector>

struct VertexBufferElement {
    u32 type;
    u32 count;
    u8 normalized;

    static u32 get_size_of_type(u32 type) {
        switch (type) {
            case GL_FLOAT: return 4;
            case GL_INT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        assert(false);

        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> elements;
    u32 stride;
    u32 divisor;
public:
    VertexBufferLayout() : stride(0), divisor(0) {}
    
    void push(u32 type, u32 count) {
        elements.push_back({ type, count, GL_FALSE });
        stride += count * VertexBufferElement::get_size_of_type(type);
    }

    void set_divisor(u32 divisor) {
        this->divisor = divisor;
    }

    const std::vector<VertexBufferElement>& get_elements() const { return elements; }
    u32 get_stride() const { return stride; }
    u32 get_divisor() const { return divisor; }
};
