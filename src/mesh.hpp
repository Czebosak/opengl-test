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
public:
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    Texture texture;

    Mesh();

    Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices, Texture texture);

    void bind() const;
    void unbind() const;
};
