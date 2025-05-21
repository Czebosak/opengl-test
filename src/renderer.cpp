#include "renderer.hpp"

void gl_clear_error() {
    while (glGetError() != GL_NO_ERROR);
}

bool gl_log_call() {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ") ヾ(ﾟдﾟ)ﾉ゛" << std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() const {
    gl_call(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
    shader.bind();
    va.bind();
    ib.bind();

    gl_call(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}
