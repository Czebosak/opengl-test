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
#include <ui.hpp>

#include <vertex_buffer_layout.hpp>
#include <vertex_buffer.hpp>
#include <index_buffer.hpp>
#include <vertex_array.hpp>

#include <elzip.hpp>
#include <miniaudio.h>

#include <beatmap.hpp>

#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float CAMERA_SPEED = 1000.0f;

ui::Context* ui_context_ptr = nullptr;

void unzip_song(const char* path) {
    elz::extractZip(path, DATA_PATH"/songs");
}

void drop_callback(GLFWwindow* window, int count, const char** paths) {
    for (int i = 0;  i < count;  i++) {
        unzip_song(paths[i]);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    int window_x, window_y;
    glfwGetWindowSize(window, &window_x, &window_y);

    float converted_y = (static_cast<float>(y) * -1.0f) + static_cast<float>(window_y);

    ui_context_ptr->handle_mouse_button_input(static_cast<float>(x), converted_y, (ui::MouseButton)button, (ui::MouseButtonAction)action);
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    Mesh mesh = Mesh::rectangle(glm::vec2(907.5f, 472.0f));
    Texture texture(ASSETS_PATH"/textures/epic.png", GL_NEAREST);

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    
    glm::mat4 projection_matrix = glm::ortho(0.0f, float(window_width), 0.0f, float(window_height));

    Shader shader(ASSETS_PATH"/shaders/texture.glsl");
    shader.bind();
    shader.set_uniform_v4("u_color", 1.0f, 1.0f, 1.0f, 1.0f);

    texture.bind();
    shader.set_uniform_1i("u_texture", 0);
    
    shader.unbind();

    Renderer renderer;

    glm::vec3 translation_a = { 400.0f, 400.0f, 0.0f };
    glm::vec3 camera_translation = { 0.0f, 0.0f, 0.0f };


    UndecodedBeatmap undecoded_beatmap = parse_osu_file("/home/czebosak/Development/cpp/graphics/opengl/osushi/data/songs/ONE OK ROCK - Start Again (A r M i N) [A r M i Nakis' Hard].osu");

    Beatmap beatmap(std::move(undecoded_beatmap));

    BeatmapPlayer beatmap_player(glm::vec2(window_width, window_height));
    
    if (auto err = beatmap_player.load_beatmap(beatmap)) {
        std::cout << *err;
        return -1;
    }

    ui::Context context(glm::vec2(window_width, window_height));
    ui_context_ptr = &context;

    Shader background_shader(ASSETS_PATH"/shaders/basic.glsl");
    background_shader.bind();
    background_shader.set_uniform_v4("u_color", 1.0f, 1.0f, 1.0f, 1.0f);

    auto epic_rectangle = std::make_unique<ui::ShaderRectangle>(
        ui::Anchor(0.2f, 0.2f, 0.2f, 0.2f),
        std::move(background_shader)
    );

    auto epic_rectangle_ptr = epic_rectangle.get();

    Shader button_shader(ASSETS_PATH"/shaders/basic.glsl");
    button_shader.bind();
    button_shader.set_uniform_v4("u_color", 0.4f, 0.4f, 0.4f, 1.0f);
    auto epic_button = std::make_unique<ui::ShaderButton>(
        ui::Anchor(0.1, 0.1, 0.9, 0.9),
        std::move(button_shader),
        [epic_rectangle_ptr](ui::Element* e, ui::MouseButton button, ui::MouseButtonAction action) {
            if (button == ui::MouseButton::LEFT) {
                epic_rectangle_ptr->shader.bind();

                if (action == ui::MouseButtonAction::PRESSED) {
                    std::cout << "Clicked! x3" << std::endl;
                    epic_rectangle_ptr->shader.set_uniform_v4("u_color", 0.5f, 0.5f, 1.0f, 1.0f);
                } else {
                    epic_rectangle_ptr->shader.set_uniform_v4("u_color", 1.0f, 1.0f, 1.0f, 1.0f);
                }
            }
        }
    );

    epic_rectangle.get()->add_child(std::move(epic_button));

    context.root.add_child(std::move(epic_rectangle));

    /* Shader background_shader2(ASSETS_PATH"/shaders/basic.glsl");
    background_shader2.bind();
    background_shader2.set_uniform_v4("u_color", 0.5f, 1.0f, 1.0f, 1.0f);

    auto epic_rectangle2 = std::make_unique<ui::ShaderRectangle>(
        ui::Anchor(0.2f, 0.2f, 0.2f, 0.2f),
        std::move(background_shader2)
    );

    epic_rectangle.get()->add_child(std::move(epic_rectangle2)); */

    ma_sound sound;
    
    ma_result result = ma_sound_init_from_file(&audio_engine, DATA_PATH"/songs/audio.mp3", 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        return result;
    }
    
    ma_sound_start(&sound);

    float delta_time = 0.0f;
    float last_frame = 0.0f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* {
            ImGui:f:Begin("Very epic move window!");

            ImGui::SliderFloat3("Translation A", &translation_a.x, -640.0f * 2.0f, 640.0f * 2.0f);

            ImGui::End();
        } */

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        beatmap_player.update(delta_time);
        beatmap_player.draw();

        //context.update();
        //context.draw(projection_matrix);
        
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
