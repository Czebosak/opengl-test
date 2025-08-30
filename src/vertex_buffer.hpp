#pragma once

#include <utils.hpp>
#include <cstddef>

class VertexBuffer {
private:
    u32 id;
public:
    VertexBuffer();
    VertexBuffer(const void* data, size_t size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void set_data(const void* data, size_t size);
    void resize(size_t new_size);
    

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) {
        id = other.id;
        other.id = 0;
    }

    VertexBuffer& operator=(VertexBuffer&& other) {
        if (this != &other) {
            id = other.id;
            other.id = 0;
        }
        return *this;
    }
};
