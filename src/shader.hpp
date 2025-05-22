#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

class Shader {
private:
    unsigned int renderer_id;
    const std::string filepath;

    std::unordered_map<std::string, int> uniform_location_cache;
    
    struct ProgramSource {
        std::string vertex_source;
        std::string fragment_source;
    };
    
    static Shader::ProgramSource parse(const std::string& filename);
    static unsigned int create(Shader::ProgramSource& source);
    static unsigned int compile(unsigned int type, const std::string& source);

    int get_uniform_location(const char* name);
public:
    Shader(const std::string& filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    // set uniforms
    void set_uniform_1i(const std::string& name, int value);
    void set_uniform_1f(const std::string& name, float value);
    void set_uniform_v4(const std::string& name, float x, float y, float z, float w);
};
