#pragma once

#include <utils.hpp>

class IndexBuffer {
private:
    u32 id;
    u32 count;
public:
    IndexBuffer();
    IndexBuffer(const u32* data, u32 count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    u32 get_count() const { return count; }

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
