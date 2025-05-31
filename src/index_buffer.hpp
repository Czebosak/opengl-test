#pragma once

class IndexBuffer {
private:
    unsigned int id;
    unsigned int count;
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int get_count() const { return count; }

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) {
        id = other.id;
        count = other.count;
        other.id = 0;
        other.id = 0;
    }

    IndexBuffer& operator=(IndexBuffer&& other) {
        if (this != &other) {
            id = other.id;
            count = other.count;
            other.id = 0;
            other.count = 0;
        }
        return *this;
    }
};
