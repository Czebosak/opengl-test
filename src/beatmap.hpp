#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct TimingPoint;
struct HitObject;

class Beatmap {  
public:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
    std::vector<TimingPoint> timing_points;
    std::vector<HitObject> hit_objects;

    void draw();
};