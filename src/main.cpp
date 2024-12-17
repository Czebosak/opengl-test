#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdio>

#include "renderer.h"
#include "shader_loader.h"

#include "vertex_buffer.h"
#include "index_buffer.h"

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        fprintf(stderr, "glfw failed to initialize ヾ(ﾟдﾟ)ﾉ゛");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640*2, 480*2, "こんにちは世界x3", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    /*glfwSwapInterval(0);*/

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "GLEW %s\nOPENGL %s\n", glewGetString(GLEW_VERSION), glGetString(GL_VERSION));

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned char indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    gl_call(glGenVertexArrays(1, &vao));
    gl_call(glBindVertexArray(vao));

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    gl_call(glEnableVertexAttribArray(0));
    gl_call(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    IndexBuffer ib(indices, 6);
    ib.bind();

    ShaderProgramSource source = parse_shader("assets/basic.glsl");

    unsigned int shader = create_shader(source.vertex_source, source.fragment_source);
    gl_call(glUseProgram(shader));

    gl_call(int location = glGetUniformLocation(shader, "u_color"));
    assert(location != -1);
    gl_call(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float increment = 0.05f;
    float delta_time = 0.0f;
    float last_frame = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        gl_call(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        gl_call(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr));

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        if (r > 1.0f) {
            increment = -5.0f * delta_time;
        } else if (r < 0.0f) {
            increment = 5.0f * delta_time;
        }

        r += increment;
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    gl_call(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
