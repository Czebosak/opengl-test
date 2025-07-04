#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "renderer.hpp"

Shader::Shader(const std::string& filepath, const std::string& mvp_uniform_name) : filepath(filepath) {
    Shader::ProgramSource source = parse(filepath);

    id = create(source);
    if (mvp_uniform_name != "") {
        mvp_uniform_location = get_uniform_location(mvp_uniform_name);
    }
}

Shader::~Shader() {
    gl_call(glDeleteProgram(id));
}

Shader::ProgramSource Shader::parse(const std::string& filename) {
    std::ifstream stream(filename);

    enum ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

u32 Shader::compile(u32 type, const std::string& source) {
    gl_call(u32 id = glCreateShader(type));
    const char* src = source.c_str();
    gl_call(glShaderSource(id, 1, &src, nullptr));
    gl_call(glCompileShader(id));

    i32 result;
    gl_call(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        i32 length;
        gl_call(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        gl_call(glGetShaderInfoLog(id, length, &length, message));
        fprintf(stderr, "Failed to compile %s shader ヾ(ﾟдﾟ)ﾉ゛\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"));
        fprintf(stderr, "%s", message);
        gl_call(glDeleteShader(id));
        return 0;
    }

    return id;
}

u32 Shader::create(Shader::ProgramSource& source) {
    gl_call(u32 program = glCreateProgram());
    u32 vs = Shader::compile(GL_VERTEX_SHADER, source.vertex_source);
    u32 fs = Shader::compile(GL_FRAGMENT_SHADER, source.fragment_source);

    gl_call(glAttachShader(program, vs));
    gl_call(glAttachShader(program, fs));
    gl_call(glLinkProgram(program));
    gl_call(glValidateProgram(program));

    gl_call(glDeleteShader(vs));
    gl_call(glDeleteShader(fs));

    return program;
}

void Shader::bind() const {
    gl_call(glUseProgram(id));
}

void Shader::unbind() const {
    gl_call(glUseProgram(0));
}

void Shader::set_mvp(const glm::mat4& matrix) {
    gl_call(glUniformMatrix4fv(mvp_uniform_location, 1, GL_FALSE, &matrix[0][0]));
}

void Shader::set_uniform_1i(const std::string& name, int value) {
    gl_call(glUniform1i(get_uniform_location(name.c_str()), value));
}

void Shader::set_uniform_1f(const std::string& name, float value) {
    gl_call(glUniform1f(get_uniform_location(name.c_str()), value));
}

void Shader::set_uniform_v2(const std::string& name, float x, float y) {
    gl_call(glUniform2f(get_uniform_location(name.c_str()), x, y));
}

void Shader::set_uniform_v4(const std::string& name, float x, float y, float z, float w) {
    gl_call(glUniform4f(get_uniform_location(name.c_str()), x, y, z, w));
}

void Shader::set_uniform_mat4f(const std::string& name, const glm::mat4& matrix) {
    gl_call(glUniformMatrix4fv(get_uniform_location(name.c_str()), 1, GL_FALSE, &matrix[0][0]));
}

i32 Shader::get_uniform_location(const std::string& name) {
    i32 location;

    auto it = uniform_location_cache.find(name);
    if (it == uniform_location_cache.end()) {
        gl_call(location = glGetUniformLocation(id, name.c_str()));
        if (location == -1) {
            std::cout << "Warning: uniform \"" << name << "\" doesn't exist!\n";
        }
        
        uniform_location_cache.emplace(name, location);
    } else {
        location = it->second;
    }

    return location;
}
