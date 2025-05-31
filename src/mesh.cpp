#include "mesh.hpp"

#include <vertex_array.hpp>
#include <vertex_buffer.hpp>
#include <vertex_buffer_layout.hpp>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<u32>&& indices, Texture texture)
    : vertices(std::move(vertices)), indices(std::move(indices)), texture(std::move(texture)) {
    /* VertexArray va;
    VertexBuffer vb(&vertices[0], sizeof(Vertex) * vertices.size());

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);
    va.add_buffer(vb, layout);

    IndexBuffer ib(&indices[0], 6); */
    vertex_buffer = VertexBuffer(&this->vertices[0], sizeof(Vertex) * this->vertices.size());

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);
    vertex_array.add_buffer(vertex_buffer, layout);

    index_buffer = IndexBuffer(&this->indices[0], this->indices.size());
}

void Mesh::bind() const {
    vertex_array.bind();
    //vertex_buffer.bind();
    index_buffer.bind();
    //texture.bind();
}

void Mesh::unbind() const {
    vertex_array.unbind();
    //vertex_buffer.unbind();
    index_buffer.unbind();
    //texture.unbind();
}

Mesh Mesh::rectangle(const glm::vec2 size, Texture texture, bool centered) {
    std::vector<Vertex> vertices = {
        { { 0.0f,   0.0f   }, { 0.0f, 0.0f } },
        { { size.x, 0.0f   }, { 1.0f, 0.0f } },
        { { size.x, size.y }, { 1.0f, 1.0f } },
        { { 0.0f,   size.y }, { 0.0f, 1.0f } }
    };

    std::vector<u32> indices = {
        0, 1, 2,
        0, 2, 3
    };

    return Mesh(std::move(vertices), std::move(indices), texture);
}
