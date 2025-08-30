#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include <vertex_buffer.hpp>
#include <index_buffer.hpp>
#include <vertex_array.hpp>
#include <shader.hpp>

#include <osu_parser.hpp>

#include <miniaudio.h>

#include <glm/glm.hpp>

struct TimingPoint;
struct HitObject;

struct Difficulty {
    float hp_drain_rate;
    float circle_size;
    float approach_rate;
    float slider_multiplier;
    float slider_tick_rate;
};

class Beatmap {
public:
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
    const std::vector<TimingPoint> timing_points;
    const std::vector<HitObject> hit_objects;

    Beatmap(const UndecodedBeatmap&& undecoded_beatmap);

    void draw();
};

struct Playfield {
    glm::vec2 screen_resolution;
    glm::vec2 size;
    glm::vec2 position;
    float osu_scale;

    glm::mat4 mvp;

    void calculate(glm::vec2 screen_resolution);
};

class BeatmapPlayer {
private:
    size_t last_circle_vector_capacity;
    std::vector<glm::ivec2> hit_object_positions;
    std::vector<float> approach_circle_values;

    VertexBuffer circle_positions_buffer;
    VertexBuffer circle_vertex_buffer;
    IndexBuffer circle_index_buffer;
    VertexArray circle_vertex_array;

    VertexBuffer approach_circle_buffer;
    VertexArray approach_circle_vertex_array;

    Shader circle_shader;
    Shader approach_circle_shader;

    const Beatmap* loaded_beatmap;
    
    Difficulty beatmap_difficulty;

    bool playing;

    float circle_diameter;

    double time;
    size_t visible_circles_index_start;
    size_t visible_circles_index_end;

    Playfield playfield;

    i32 approach_time;

    ma_engine& audio_engine;
    ma_sound music;

    std::optional<std::string> fill_beatmap_difficulty();

    void calculate_approach_time();
    void calculate_circle_diameter();
    void calculate_visible_circles();
public:
    BeatmapPlayer(glm::vec2 screen_resolution, ma_engine& audio_engine);
    
    std::optional<std::string> load_beatmap(const Beatmap& beatmap);

    void reset();
    int start();
    void stop();
    
    void update(double delta);

    void draw();

    bool is_playing();
};
