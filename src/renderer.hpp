#pragma once

#include <cassert>

#include <GL/glew.h>
#include <iostream>

#include <index_buffer.hpp>
#include <vertex_array.hpp>
#include <shader.hpp>

void gl_clear_error();

bool gl_log_call();

/* #ifdef DEBUG */
    #define gl_call(x) gl_clear_error();\
        x;\
        assert(gl_log_call())
/* #else
    #define gl_call(x) x
#endif */

class Renderer {
public:
    void clear() const;

    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
