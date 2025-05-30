#include "sprite.hpp"

#include <numeric>

/*
Sprite::Sprite(const Texture texture, const glm::mat4 transform, Anchor anchor, const Shader shader) : shader(SPRITE_SHADER_PATH) {}

Sprite::Sprite(const Texture texture, const glm::vec2 translation, float rotation, Anchor anchor, const Shader shader) : translation(translation), rotation(rotation), shader(shader) {
    int width = texture.get_width();
    int height = texture.get_height();
    glm::vec2 size(width, height);

    std::vector<Vertex> vertices(4);

    // Left up
    vertices.emplace_back(
        Vertex {
            (anchor == Anchor::Center) ? (size / -2.0f) : glm::vec2(0.0f),
            glm::vec2(0.0f)
        }
    );
    // Right up
    vertices.emplace_back(
        Vertex {
            (anchor == Anchor::Center) ? glm::vec2(size.x / 2.0f, size.y / -2.0f) : glm::vec2(0.0f),
            glm::vec2(1.0f, 0.0f)
        }
    );
    // Left down
    vertices.emplace_back(
        Vertex {
            (anchor == Anchor::Center) ? glm::vec2(size.x / -2.0f, size.y / 2.0f) : glm::vec2(0.0f),
            glm::vec2(1.0f, 0.0f)
        }
    );
    // Right down
    vertices.emplace_back(
        Vertex {
            (anchor == Anchor::Center) ? (size / 2.0f) : glm::vec2(0.0f),
            glm::vec2(1.0f)
        }
    );

    // Fill indices vector to form a rectangle
    std::vector<u32> indices = {
        0, 1, 2,
        0, 2, 3
    };

    mesh = Mesh(std::move(vertices), std::move(indices), { texture });
}

void Sprite::render(const Renderer& renderer) {
    mesh.bind();
    shader.bind();
} */
