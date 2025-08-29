#include "texture.hpp"

Texture::Texture(const std::string& path, GLenum filter, GLenum internal_format, GLenum format) : file_path(path) {
    stbi_set_flip_vertically_on_load(true);
    u8* local_buffer = stbi_load(file_path.c_str(), &width, &height, &bpp, 4);

    gl_call(glGenTextures(1, &id));
    gl_call(glBindTexture(GL_TEXTURE_2D, id));

    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gl_call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    gl_call(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, local_buffer));
    unbind();

    if (local_buffer) {
        stbi_image_free(local_buffer);
    }
}

Texture::~Texture() {
}

void Texture::bind(unsigned int slot) const {
    gl_call(glActiveTexture(GL_TEXTURE0 + slot));
    gl_call(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::unbind() const {
    gl_call(glBindTexture(GL_TEXTURE_2D, 0));
}
