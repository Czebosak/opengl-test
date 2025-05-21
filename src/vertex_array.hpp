#pragma once

#include <vertex_buffer.hpp>

class VertexBufferLayout;

class VertexArray {
private:
    unsigned int renderer_id;
public:
    VertexArray();
    ~VertexArray();

    void add_buffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void bind() const;
    void unbind() const;
};

