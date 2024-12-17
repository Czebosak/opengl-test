#include "renderer.h"

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
