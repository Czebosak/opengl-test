#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include <utils.hpp>

struct UndecodedBeatmap {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
    std::vector<std::string> timing_points;
    std::vector<std::string> hit_objects;
};

struct TimingPoint {
    i32 time;
    float beat_length;
    i32 meter;
    i32 sample_set;
    i32 sample_index;
    i32 volume;
    bool uninherited;
    i32 effects;
};

struct HitObject {};

UndecodedBeatmap parse_osu_file(std::string filepath);

std::vector<TimingPoint> parse_timing_points(const std::vector<std::string>& data);

std::vector<HitObject> parse_hit_objects(const std::vector<std::string>& data);
