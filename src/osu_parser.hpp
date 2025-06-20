#pragma once
#include <vector>
#include <unordered_map>
#include <string>


struct UndecodedBeatmap {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
    std::vector<std::string> timing_points;
    std::vector<std::string> hit_objects;
};

UndecodedBeatmap parse_osu_file(std::string filepath);
