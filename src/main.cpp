#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <cstdio>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <renderer.hpp>
#include <shader.hpp>
#include <texture.hpp>

#include <vertex_buffer_layout.hpp>
#include <vertex_buffer.hpp>
#include <index_buffer.hpp>
#include <vertex_array.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    float positions[] = {
        0.0f,   0.0f,   0.0f, 0.0f,
        907.5f, 0.0f,   1.0f, 0.0f,
        907.5f, 472.0f, 1.0f, 1.0f,
        0.0f,   472.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    gl_call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    gl_call(glEnable(GL_BLEND));

    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);
    va.add_buffer(vb, layout);

    IndexBuffer ib(indices, 6);

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    
    glm::mat4 projection_matrix = glm::ortho(0.0f, float(window_width), 0.0f, float(window_height), -1.0f, 1.0f);

    Shader shader(ASSETS_PATH"/shaders/basic.glsl");
    shader.bind();

    shader.set_uniform_v4("u_color", 1.0f, 1.0f, 1.0f, 1.0f);

    Texture texture(ASSETS_PATH"/textures/epic.png", GL_NEAREST);
    texture.bind();
    shader.set_uniform_1i("u_texture", 0);

    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    Renderer renderer;

    glm::vec3 translation_a = { 400.0f, 400.0f, 0.0f };
    glm::vec3 translation_b = { 0.0f, 0.0f, 0.0f };
    glm::vec3 camera_translation = { 0.0f, 0.0f, 0.0f };

    float delta_time = 0.0f;
    float last_frame = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Very epic move window!");

        float v[3];
        ImGui::SliderFloat3("Translation A", &translation_a.x, -640.0f * 2.0f, 640.0f * 2.0f);
        ImGui::SliderFloat3("Translation B", &translation_b.x, -640.0f * 2.0f, 640.0f * 2.0f);
        ImGui::SliderFloat3("Camera Translation", &camera_translation.x, -640.0f * 2.0f, 640.0f * 2.0f);

        ImGui::End();
        
        glm::mat4 view = glm::translate(glm::mat4(1.0f), camera_translation);

        shader.bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation_a);
            glm::mat4 mvp = projection_matrix * view * model;

            shader.set_uniform_mat4f("u_mpv", mvp);
            renderer.draw(va, ib, shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation_b);
            glm::mat4 mvp = projection_matrix * view * model;

            shader.set_uniform_mat4f("u_mpv", mvp);
            renderer.draw(va, ib, shader);
        }

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
