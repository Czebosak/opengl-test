#include "osu_parser.hpp"

#include <fstream>

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
                beatmap.timing_points.emplace_back(line);
            } else if (current_section == "HitObjects") {
                beatmap.hit_objects.emplace_back(line);
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
