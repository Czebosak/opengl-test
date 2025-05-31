#pragma once
#include <string>

#include <stb_image.h>
#include <renderer.hpp>

class Texture {
private:
    unsigned int id;
    std::string file_path;
    int width, height, bpp;
public:
    Texture(const std::string& path, GLenum filter = GL_LINEAR);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int get_width() const { return width; }
    inline int get_height() const { return height; }
};