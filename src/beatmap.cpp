#include "beatmap.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <vertex_buffer_layout.hpp>
#include <renderer.hpp>
#include <mesh.hpp>

#include <algorithm>

const size_t INITIAL_HIT_OBJECT_CAPACITY = 20;

Beatmap::Beatmap(const UndecodedBeatmap&& undecoded_beatmap)
: data(std::move(undecoded_beatmap.data)), timing_points(parse_timing_points(undecoded_beatmap.timing_points)), hit_objects(parse_hit_objects(undecoded_beatmap.hit_objects)) {}

BeatmapPlayer::BeatmapPlayer(glm::vec2 screen_resolution, ma_engine& audio_engine) : audio_engine(audio_engine), circle_shader(ASSETS_PATH"/shaders/circle.glsl"), approach_circle_shader(ASSETS_PATH"/shaders/approach_circle.glsl") {
    last_circle_vector_capacity = 0;
    visible_circles_index_start = 0;
    visible_circles_index_end   = 0;
    time = 0.0;

    Vertex vertices[4] = {
        { { -0.5f, -0.5f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f }, { 1.0f, 0.0f } },
        { {  0.5f,  0.5f }, { 1.0f, 1.0f } },
        { { -0.5f,  0.5f }, { 0.0f, 1.0f } }
    };

    u32 indices[6] = {
        0, 1, 2,
        0, 2, 3
    };

    circle_vertex_buffer = VertexBuffer(vertices, 4 * sizeof(Vertex));
    circle_index_buffer = IndexBuffer(indices, 6);

    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);

    circle_vertex_array.add_buffer(circle_vertex_buffer, layout);

    hit_object_positions.reserve(INITIAL_HIT_OBJECT_CAPACITY);
    circle_positions_buffer = VertexBuffer(nullptr, INITIAL_HIT_OBJECT_CAPACITY * sizeof(glm::ivec2));
    last_circle_vector_capacity = INITIAL_HIT_OBJECT_CAPACITY;

    VertexBufferLayout instance_layout;
    instance_layout.set_divisor(1);
    instance_layout.push(GL_INT, 2);

    circle_vertex_array.add_buffer(circle_positions_buffer, instance_layout);

    approach_circle_buffer = VertexBuffer(nullptr, INITIAL_HIT_OBJECT_CAPACITY * sizeof(float));
    
    approach_circle_vertex_array.add_buffer(circle_vertex_buffer, layout);
    approach_circle_vertex_array.add_buffer(circle_positions_buffer, instance_layout);

    VertexBufferLayout approach_instance_layout;
    approach_instance_layout.set_divisor(1);
    approach_instance_layout.push(GL_FLOAT, 1);

    approach_circle_vertex_array.add_buffer(approach_circle_buffer, approach_instance_layout);

    playfield.calculate(screen_resolution);

    circle_shader.bind();
    circle_shader.set_mvp(playfield.mvp);
    approach_circle_shader.bind();
    approach_circle_shader.set_mvp(playfield.mvp);
}

bool convert_string_to_float(const std::string& s, float& n) {
    return std::from_chars(s.data(), s.data() + s.size(), n).ec == std::errc();
}

std::optional<std::string> BeatmapPlayer::fill_beatmap_difficulty() {
    const auto& difficulty = loaded_beatmap->data.at("Difficulty");
    
    if (
        !(
            convert_string_to_float(difficulty.at("HPDrainRate"), beatmap_difficulty.hp_drain_rate) &&
            convert_string_to_float(difficulty.at("CircleSize"), beatmap_difficulty.circle_size) &&
            convert_string_to_float(difficulty.at("ApproachRate"), beatmap_difficulty.approach_rate) &&
            convert_string_to_float(difficulty.at("SliderMultiplier"), beatmap_difficulty.slider_multiplier) &&
            convert_string_to_float(difficulty.at("SliderTickRate"), beatmap_difficulty.slider_tick_rate)
        )
    ) {
        return std::make_optional("Incorrect OSU beatmap\n");
    }

    return std::nullopt;
}

void Playfield::calculate(glm::vec2 screen_resolution) {
    size.y = screen_resolution.y * 0.8f;
    size.x = ((4.0f / 3.0f) * size.y);
    position = screen_resolution / 2.0f;
    position.y -= size.y * 0.02f;

    glm::vec2 osu_scale2d = size / glm::vec2(512.0f, 384.0f);
    osu_scale = std::min(osu_scale2d.x, osu_scale2d.y);


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position - (size / 2.0f), 0.0f));
    model = glm::scale(model, glm::vec3(osu_scale, osu_scale, 1.0f));

    glm::mat4 proj = glm::ortho(0.0f, screen_resolution.x, screen_resolution.y, 0.0f);

    mvp = proj * model;
}

void BeatmapPlayer::calculate_approach_time() {
    float approach_rate = beatmap_difficulty.approach_rate;

    approach_time = 1800;

    approach_time -= std::min(approach_rate, 5.0f) * 120.0f;
    if (approach_rate > 5.0f) approach_time -= (approach_rate - 5.0f) * 150.0f;
}

void BeatmapPlayer::calculate_circle_diameter() {
    float circle_radius = 54.4f - 4.48f * beatmap_difficulty.circle_size;
    circle_diameter = circle_radius * 2.0f;
    circle_shader.bind();
    circle_shader.set_uniform_1f("circle_diameter", circle_diameter);
    approach_circle_shader.bind();
    approach_circle_shader.set_uniform_1f("circle_diameter", circle_diameter);
}

void BeatmapPlayer::calculate_visible_circles() {
    const std::vector<HitObject>& hit_objects = loaded_beatmap->hit_objects;

    hit_object_positions.clear();

    i32 song_time = round(time);

    while (visible_circles_index_start < hit_objects.size()) {
        if (hit_objects[visible_circles_index_start].time > (song_time - approach_time)) {
            break;
        }
        visible_circles_index_start++;
    }

    while (visible_circles_index_end < hit_objects.size()) {
        if (hit_objects[visible_circles_index_end].time > song_time) {
            break;
        }
        visible_circles_index_end++;
    }

    if (visible_circles_index_end > visible_circles_index_start) {
        size_t length = (visible_circles_index_end - visible_circles_index_start);
        hit_object_positions.resize(length);
        approach_circle_values.resize(length);

        for (int i = 0; i < length; i++) {
            const HitObject& hit_object = hit_objects[i + visible_circles_index_start];
            hit_object_positions[i].x = hit_object.x;
            hit_object_positions[i].y = hit_object.y;

            float remain = (float)(hit_object.time - song_time + approach_time);
            float a = remain / approach_time;
            approach_circle_values[i] = std::clamp(a, 0.001f, 2.0f);
        }
    }

    if (last_circle_vector_capacity != hit_object_positions.capacity()) {
        circle_positions_buffer.bind();
        circle_positions_buffer.resize(hit_object_positions.capacity() * sizeof(glm::ivec2));

        approach_circle_buffer.bind();
        approach_circle_buffer.resize(hit_object_positions.capacity() * sizeof(float));

        last_circle_vector_capacity = hit_object_positions.capacity();
    }

    circle_positions_buffer.bind();
    circle_positions_buffer.set_data(hit_object_positions.data(), hit_object_positions.size() * sizeof(glm::ivec2));

    approach_circle_buffer.bind();
    approach_circle_buffer.set_data(approach_circle_values.data(), approach_circle_values.size() * sizeof(float));
}

std::optional<std::string> BeatmapPlayer::load_beatmap(const Beatmap& beatmap) {
    loaded_beatmap = &beatmap;

    // Fill out difficulty struct
    if (auto err = fill_beatmap_difficulty()) return err;

    calculate_approach_time();
    calculate_circle_diameter();

    return std::nullopt;
}

int BeatmapPlayer::start() {
    visible_circles_index_start = 0;
    visible_circles_index_end   = 0;
    time = 0.0;
    
    std::string filename = loaded_beatmap->data.at("General").at("AudioFilename");
    ma_result result = ma_sound_init_from_file(&audio_engine, (std::string(DATA_PATH) + "/songs/" + filename).c_str(), 0, NULL, NULL, &music);
    if (result != MA_SUCCESS) {
        return result;
    }
    
    ma_sound_start(&music);
    return 0;
}

void BeatmapPlayer::update(double delta) {
    time += delta * 1000.0 * 1.0;

    calculate_visible_circles();
}

void BeatmapPlayer::draw() {
    if (hit_object_positions.size() > 0) {
        circle_vertex_array.bind();
        circle_index_buffer.bind();
        circle_shader.bind();
        gl_call(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, hit_object_positions.size()));

        approach_circle_vertex_array.bind();
        circle_index_buffer.bind();
        approach_circle_shader.bind();
        gl_call(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, approach_circle_values.size()));
    }
}

void beatmap_loop() {
    //ma_sound_uninit(&sound);
}
