#include "osu_parser.hpp"

#include <fstream>
#include <string_view>
#include <charconv>
#include <cctype>
#include <iostream>

ObjectType::ObjectType(u8 byte) {
    circle     = byte & (1 << 0);
    slider     = byte & (1 << 1);
    combo_mark = byte & (1 << 2);
    spinner    = byte & (1 << 3);
    color_hax  = (byte >> 4) & 0b111;
    hold_note  = byte & (1 << 7);
}

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

template <std::size_t N>
std::array<std::string_view, N> split_by_comma(std::string_view s) {
    std::array<std::string_view, N> split_data;

    size_t last_idx = 0;
    for (size_t j = 0; j < split_data.size(); j++) {
        size_t idx;
        if (j == 7) {
            idx == s.size() - 1;
        } else {
            idx = s.find(',', last_idx);
        }

        if (idx == std::string::npos) {
            printf("Invalid OSU file syntax\n");
            return {};
        }

        split_data[j] = s.substr(last_idx, idx - last_idx);
        last_idx = idx + 1;
    }

    return split_data;
}

#define CONVERT(var) (i++, std::from_chars(split_data[i - 1].begin(), split_data[i - 1].end(), var).ec == std::errc())

#define TIMING_POINT_VALUE_NUMBER 8
std::vector<TimingPoint> parse_timing_points(const std::vector<std::string>& data) {
    std::vector<TimingPoint> timing_points;
    timing_points.reserve(data.size());

    for (const std::string_view s : data) {
        std::array<std::string_view, TIMING_POINT_VALUE_NUMBER> split_data = split_by_comma<TIMING_POINT_VALUE_NUMBER>(s);

        TimingPoint timing_point;

        int i = 0;

        u32 temp_uninherited;

        if (
            !(
                CONVERT(timing_point.time) &&
                CONVERT(timing_point.beat_length) &&
                CONVERT(timing_point.meter) &&
                CONVERT(timing_point.sample_set) &&
                CONVERT(timing_point.sample_index) &&
                CONVERT(timing_point.volume) &&
                CONVERT(temp_uninherited) &&
                CONVERT(timing_point.effects)
            )
        ) {
            printf("Couldn't convert type while parsing timing points\n");
            return {};
        }

        timing_point.uninherited = static_cast<bool>(temp_uninherited);

        timing_points.push_back(timing_point);
    }

    return timing_points;
}

#define HIT_OBJECT_VALUE_NUMBER 5 // temporary normally its 7
std::vector<HitObject> parse_hit_objects(const std::vector<std::string>& data) {
    std::vector<HitObject> hit_objects;
    hit_objects.reserve(data.size());

    for (const std::string_view s : data) {
        std::array<std::string_view, HIT_OBJECT_VALUE_NUMBER> split_data = split_by_comma<HIT_OBJECT_VALUE_NUMBER>(s);
        
        HitObject hit_object;

        int i = 0;

        u8 temp_type;

        i32 x;
        i32 y;
        i32 time;
        i32 hit_sound;

        /* CONVERT(hit_object.x);
        CONVERT(hit_object.y);
        CONVERT(hit_object.time);
        CONVERT(temp_type);
        CONVERT(hit_object.hit_sound); */

        i = 0;

        if (
            !(
                CONVERT(hit_object.x) &&
                CONVERT(hit_object.y) &&
                CONVERT(hit_object.time) &&
                CONVERT(temp_type) &&
                CONVERT(hit_object.hit_sound)
            )
        ) {
            printf("Couldn't convert type while parsing timing points\n");
            return {};
        }

        hit_object.type = ObjectType(temp_type);

        hit_objects.push_back(hit_object);
    }
    
    return hit_objects;
}
