#pragma once

#include <GL/glew.h>
#include <iostream>

void gl_clear_error();

bool gl_log_call();

#ifdef DEBUG
    #define gl_call(x) gl_clear_error();\
        x;\
        assert(gl_log_call())
#else
    #define gl_call(x) x
#endif
