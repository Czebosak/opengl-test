#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdio>

#include "renderer.hpp"
#include "shader.hpp"

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "vertex_array.hpp"

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
    glfwSwapInterval(0);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
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

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    va.add_buffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader(ASSETS_PATH"/basic.glsl");
    shader.bind();

    shader.set_uniform_v4("u_color", 0.2f, 0.3f, 0.8f, 1.0f);

    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    float r = 0.0f;
    float increment = 0.05f;
    float delta_time = 0.0f;
    float last_frame = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        gl_call(glClear(GL_COLOR_BUFFER_BIT));

        va.bind();
        ib.bind();

        shader.bind();
        shader.set_uniform_v4("u_color", r, 0.3f, 0.8f, 1.0f);
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

    glfwTerminate();
    return 0;
}
