#pragma once

#include <vertex_buffer.hpp>

#include <utils.hpp>

class VertexBufferLayout;

class VertexArray {
private:
    u32 id;
    int attribute_index;
public:
    VertexArray();
    ~VertexArray();

    void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void bind() const;
    void unbind() const;
};

