#pragma once
#include <string>
#include <string_view>

class Shader {
private:
    unsigned int renderer_id;
    const std::string filepath;
    
    struct ProgramSource {
        std::string vertex_source;
        std::string fragment_source;
    };
    
    static Shader::ProgramSource parse(const std::string& filename);
    static unsigned int create(Shader::ProgramSource& source);
    static unsigned int compile(unsigned int type, const std::string& source);

    int get_uniform_location(const char* name);
public:
    Shader(const std::string_view filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    // set uniforms
    void set_uniform_v4(const std::string& name, float x, float y, float z, float w);
};
