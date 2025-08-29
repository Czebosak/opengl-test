#pragma once
#include <string>
#include <string_view>
#include <unordered_map>

#include <utils.hpp>

#include <glm/glm.hpp>

class Shader {
private:
    u32 id;
    std::string filepath;

    i32 mvp_uniform_location;

    std::unordered_map<std::string, i32> uniform_location_cache;
    
    struct ProgramSource {
        std::string vertex_source;
        std::string fragment_source;
    };
    
    static Shader::ProgramSource parse(const std::string& filename);
    static u32 create(Shader::ProgramSource& source);
    static u32 compile(u32 type, const std::string& source);

    i32 get_uniform_location(const std::string& name);
public:
    Shader(const std::string& filepath, const std::string& mvp_uniform_name = "mvp");
    ~Shader();

    void bind() const;
    void unbind() const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) {
        id = other.id;
        other.id = 0;
        filepath = std::move(other.filepath);
        uniform_location_cache = std::move(other.uniform_location_cache);

        mvp_uniform_location = other.mvp_uniform_location;
    }

    Shader& operator=(Shader&& other) {
        if (this != &other) {
            id = other.id;
            other.id = 0;
            filepath = std::move(other.filepath);
            uniform_location_cache = std::move(other.uniform_location_cache);

            mvp_uniform_location = other.mvp_uniform_location;
        }
        return *this;
    }

    // common uniforms
    void set_mvp(const glm::mat4& matrix);

    // set uniforms
    void set_uniform_1i(const std::string& name, int value);
    void set_uniform_1f(const std::string& name, float value);
    void set_uniform_v2(const std::string& name, float x, float y);
    void set_uniform_v4(const std::string& name, float x, float y, float z, float w);
    void set_uniform_mat4f(const std::string& name, const glm::mat4& matrix);
};
