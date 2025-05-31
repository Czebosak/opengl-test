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
    Texture texture;
public:
    Mesh();

    Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices, Texture texture);

    static Mesh rectangle(const glm::vec2 size, Texture texture, bool centered = false);

    void bind() const;
    void unbind() const;

    VertexArray& get_vertex_array() { return vertex_array; }
    VertexBuffer& get_vertex_buffer() { return vertex_buffer; }
    IndexBuffer& get_index_buffer() { return index_buffer; }
    Texture& get_texture() { return texture; }
};
