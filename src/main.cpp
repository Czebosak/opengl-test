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
#include <sprite.hpp>

#include <vertex_buffer_layout.hpp>
#include <vertex_buffer.hpp>
#include <index_buffer.hpp>
#include <vertex_array.hpp>

#include <elzip.hpp>
#include <miniaudio.h>

#include <osu_parser.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float CAMERA_SPEED = 1000.0f;

void unzip_song(const char* path) {
    elz::extractZip(path, DATA_PATH"/songs");
}

void drop_callback(GLFWwindow* window, int count, const char** paths) {
    for (int i = 0;  i < count;  i++) {
        unzip_song(paths[i]);
    }
}

void song_frame(float delta) {

}

GLFWwindow* setup_window_and_context(u32 width, u32 height, const char* title) {
    /* Initialize the library */
    if (!glfwInit()) {
        fprintf(stderr, "glfw failed to initialize ヾ(ﾟдﾟ)ﾉ゛");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        fprintf(stderr, "Window failed to initialize ヾ(ﾟдﾟ)ﾉ゛");
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "GLEW %s\nOPENGL %s\n", glewGetString(GLEW_VERSION), glGetString(GL_VERSION));

    // Setup ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    
    // Setup blending
    gl_call(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    gl_call(glEnable(GL_BLEND));

    return window;
}

int main(void) {
    GLFWwindow* window = setup_window_and_context(640*2, 480*2, "こんにちは世界x3");
    if (!window) {
        return -1;
    }

    ma_engine audio_engine;

    {
        ma_result result = ma_engine_init(NULL, &audio_engine);
        if (result != MA_SUCCESS) {
            return result;  // Failed to initialize the engine.
        }
    }

    glfwSetDropCallback(window, drop_callback);

    Mesh mesh = Mesh::rectangle(glm::vec2(907.5f, 472.0f), Texture(ASSETS_PATH"/textures/epic.png", GL_NEAREST));

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    
    glm::mat4 projection_matrix = glm::ortho(0.0f, float(window_width), 0.0f, float(window_height), -1.0f, 1.0f);

    Shader shader(ASSETS_PATH"/shaders/texture.glsl");
    shader.bind();
    shader.set_uniform_v4("u_color", 1.0f, 1.0f, 1.0f, 1.0f);

    mesh.get_texture().bind();
    shader.set_uniform_1i("u_texture", 0);
    
    shader.unbind();

    Renderer renderer;

    glm::vec3 translation_a = { 400.0f, 400.0f, 0.0f };
    glm::vec3 camera_translation = { 0.0f, 0.0f, 0.0f };

    /* UndecodedBeatmap beatmap = parse_osu_file("/home/czebosak/Development/cpp/graphics/opengl/osushi/data/songs/ONE OK ROCK - Start Again (A r M i N) [A r M i Nakis' Hard].osu");
    for (auto& kv : beatmap.data) {
        std::cout << "Section: " << kv.first << "\n";
        for (auto& kv : kv.second) {
            std::cout << kv.first << ": " << kv.second << "\n";
        }
    }
    std::cout << "\nTimestamps\n";
    for (std::string& timestamp : beatmap.timing_points) {
        std::cout << timestamp << "\n";
    }
    std::cout << "\nHitObjects\n";
    for (std::string& timestamp : beatmap.hit_objects) {
        std::cout << timestamp << "\n";
    } */

    ma_sound sound;

    
    printf(DATA_PATH"/audio.mp3\n");
    ma_result result = ma_sound_init_from_file(&audio_engine, DATA_PATH"/songs/audio.mp3", 0, NULL, NULL, &sound);
    printf("WEEE ARE HEEERE\n");
    if (result != MA_SUCCESS) {
        printf("WEEE ARE HEEERE4\n");
        return result;
        printf("WEEE ARE HEEERE5\n");
    }

    printf("WEEE ARE HEEERE3\n");
    ma_sound_start(&sound);
    printf("WEEE ARE HEEERE2\n");

    float delta_time = 0.0f;
    float last_frame = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Very epic move window!");

            ImGui::SliderFloat3("Translation A", &translation_a.x, -640.0f * 2.0f, 640.0f * 2.0f);

            ImGui::End();
        }

        {
            glm::vec3 input(0.0f);
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                input.y -= 1.0f;
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                input.y += 1.0f;
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                input.x += 1.0f;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                input.x -= 1.0f;
            }

            if (glm::length(input) != 0.0f) {
                camera_translation += glm::normalize(input) * CAMERA_SPEED * delta_time;
            }
        }
        
        glm::mat4 view = glm::translate(glm::mat4(1.0f), camera_translation);

        shader.bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation_a);
            glm::mat4 mvp = projection_matrix * view * model;

            shader.set_mvp(mvp);
            mesh.bind();
            mesh.draw();
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

    ma_sound_uninit(&sound);

    ma_engine_uninit(&audio_engine);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
