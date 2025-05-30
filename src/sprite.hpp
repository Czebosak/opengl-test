#pragma once
#include <renderer.hpp>
#include <shader.hpp>
#include <mesh.hpp>

#include <glm/glm.hpp>

#include <utils.hpp>

#define SPRITE_SHADER_PATH ASSETS_PATH"/shaders/basic.glsl"

class Sprite {
private:
    Mesh mesh;
    Shader shader;
    glm::mat4 transform;
    glm::vec2 translation;
    float rotation;
public:
    enum Anchor : u8 {
        Center,
        Corner
    };

    Sprite(const Texture texture, const glm::mat4 transform, Anchor anchor = Anchor::Center, const Shader shader = Shader(SPRITE_SHADER_PATH));
    Sprite(const Texture texture, const glm::vec2 translation = glm::vec2(0.0f), float rotation = 0.0f, Anchor anchor = Anchor::Center, const Shader shader = Shader(SPRITE_SHADER_PATH));

    void render(const Renderer& renderer);

    inline Shader& get_shader() { return shader; }
    inline Mesh& get_mesh() { return mesh; }
    inline const glm::mat4& get_transform() const { return transform; }
    inline glm::vec2 get_translation() const { return translation; }
    inline float get_rotation() const { return rotation; }

    void set_transform();
    void set_translation();
    void set_rotation();
};
