#pragma once
#include <GL/glew.h>

#include "utils.hpp"
#include <assert.h>
#include <vector>

struct VertexBufferElement {
    u32 type;
    u32 count;
    unsigned char normalized;

    static unsigned int get_size_of_type(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
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
public:
    VertexBufferLayout() : stride(0) {}
    
    void push(unsigned int type, unsigned int count) {
        elements.push_back({ type, count, GL_FALSE });
        stride += count * VertexBufferElement::get_size_of_type(type);
    }

    inline const std::vector<VertexBufferElement>& get_elements() const { return elements; }
    inline u32 get_stride() const { return stride; }
};
