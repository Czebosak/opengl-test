#pragma once

class VertexBuffer {
private:
    unsigned int id;
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

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
