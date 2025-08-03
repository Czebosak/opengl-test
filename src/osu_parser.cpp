#include "osu_parser.hpp"

#include <fstream>
#include <string_view>
#include <charconv>
#include <cctype>
#include <iostream>

UndecodedBeatmap parse_osu_file(std::string filepath) {
    UndecodedBeatmap beatmap;

    std::ifstream stream(filepath);

    std::string current_section;
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) { continue; }

        if (line.starts_with("[")) {
            current_section = line.substr(1, line.length() - 3);
        } else if (!current_section.empty()) {
            if (current_section == "TimingPoints") { 
                if (!std::isspace(line[0])) {
                    beatmap.timing_points.emplace_back(line);
                }
            } else if (current_section == "HitObjects") {
                if (!std::isspace(line[0])) {
                    beatmap.hit_objects.emplace_back(line);
                }
            } else {
                std::string key;
                int i = 0;
                for (i; i < line.length(); i++) {
                    if (line[i] == ':') { i++; break; };
                    key += line[i];
                }

                std::string value;
                for (i; i < line.length(); i++) {
                    value += line[i];
                }
                if (value.starts_with(' ')) {
                    value = value.substr(1, value.length() - 2);
                }

                beatmap.data[current_section][key] = value;
            }
        }
    }

    return beatmap;
}

std::vector<TimingPoint> parse_timing_points(const std::vector<std::string>& data) {
    std::vector<TimingPoint> timing_points;
    timing_points.reserve(data.size());

    for (const std::string_view s : data) {
        std::array<std::string_view, 8> split_data;
        size_t last_idx = 0;
        for (size_t j = 0; j < split_data.size(); j++) {
            size_t idx;
            if (j == 7) {
                idx == s.size() - 1;
            } else {
                idx = s.find(',', last_idx);
            }

            if (idx == std::string::npos) {
                printf("Invalid timing point syntax\n");
                return {};
            }

            split_data[j] = s.substr(last_idx, idx - last_idx);
            last_idx = idx + 1;
        }

        TimingPoint timing_point;

        int i = 0;

        #define CONVERT(var) (i++, std::from_chars(split_data[i - 1].begin(), split_data[i - 1].end(), var).ec == std::errc())

        i32 temp_uninherited;

        if (
            !CONVERT(timing_point.time) &&
            !CONVERT(timing_point.beat_length) &&
            !CONVERT(timing_point.meter) &&
            !CONVERT(timing_point.sample_set) &&
            !CONVERT(timing_point.sample_index) &&
            !CONVERT(timing_point.volume) &&
            !CONVERT(temp_uninherited) &&
            !CONVERT(timing_point.effects)
        ) {
            printf("Couldn't convert type while parsing timing points\n");
            return {};
        }

        timing_point.uninherited = static_cast<bool>(temp_uninherited);

        timing_points.push_back(timing_point);
    }

    return timing_points;
}

std::vector<HitObject> parse_hit_objects(const std::vector<std::string>& data) {
    std::vector<HitObject> hit_objects;
    hit_objects.reserve(data.size());

    for (size_t i = 0; i < data.size(); i++) {
        const std::string& s = data[i];
    }
    
    return hit_objects;
}
