#pragma once

#include <unordered_map>

#include <glm/glm.hpp>

#include <utils.hpp>

#include <shader.hpp>

struct Character {
    u32 texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer {
private:
    std::unordered_map<u8, Character> characters;
public:
    TextRenderer() {}

    int load_font(const char* file_path, unsigned int font_size);

    void render_text(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
};
