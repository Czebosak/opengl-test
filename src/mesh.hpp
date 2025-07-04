#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <utils.hpp>
#include <texture.hpp>

struct Vertex {
    glm::vec2 position;
    glm::vec2 uv;
};

class VertexArray;
class VertexBuffer;
class VertexBufferLayout;

class Mesh {
private:
    VertexArray vertex_array;
    VertexBuffer vertex_buffer;
    IndexBuffer index_buffer;

    std::vector<Vertex> vertices;
    std::vector<u32> indices;
public:
    Mesh();

    Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices);

    static Mesh rectangle(const glm::vec2 size, bool centered = false);

    void bind() const;
    void unbind() const;

    void draw() const;

    VertexArray& get_vertex_array() { return vertex_array; }
    VertexBuffer& get_vertex_buffer() { return vertex_buffer; }
    IndexBuffer& get_index_buffer() { return index_buffer; }
};
