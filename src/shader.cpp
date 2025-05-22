#include "shader.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "renderer.hpp"

Shader::Shader(const std::string& filepath) : filepath(filepath) {
    Shader::ProgramSource source = parse(filepath);

    renderer_id = create(source);
}

Shader::~Shader() {
    gl_call(glDeleteProgram(renderer_id));
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

unsigned int Shader::compile(unsigned int type, const std::string& source) {
    gl_call(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    gl_call(glShaderSource(id, 1, &src, nullptr));
    gl_call(glCompileShader(id));

    int result;
    gl_call(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
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

unsigned int Shader::create(Shader::ProgramSource& source) {
    gl_call(unsigned int program = glCreateProgram());
    unsigned int vs = Shader::compile(GL_VERTEX_SHADER, source.vertex_source);
    unsigned int fs = Shader::compile(GL_FRAGMENT_SHADER, source.fragment_source);

    gl_call(glAttachShader(program, vs));
    gl_call(glAttachShader(program, fs));
    gl_call(glLinkProgram(program));
    gl_call(glValidateProgram(program));

    gl_call(glDeleteShader(vs));
    gl_call(glDeleteShader(fs));

    return program;
}

void Shader::bind() const {
    gl_call(glUseProgram(renderer_id));
}

void Shader::unbind() const {
    gl_call(glUseProgram(0));
}

void Shader::set_uniform_1i(const std::string& name, int value) {
    gl_call(glUniform1i(get_uniform_location(name.c_str()), value));
}

void Shader::set_uniform_1f(const std::string& name, float value) {
    gl_call(glUniform1f(get_uniform_location(name.c_str()), value));
}

void Shader::set_uniform_v4(const std::string& name, float x, float y, float z, float w) {
    gl_call(glUniform4f(get_uniform_location(name.c_str()), x, y, z, w));
}

int Shader::get_uniform_location(const char* name) {
    int location;

    std::string id = std::string(name);
    auto it = uniform_location_cache.find(id);
    if (it == uniform_location_cache.end()) {
        gl_call(location = glGetUniformLocation(renderer_id, name));
        if (location == -1) {
            std::cout << "Warning: uniform \"" << name << "\" doesn't exist!\n";
        }
        
        uniform_location_cache.emplace(std::move(id), location);
    } else {
        location = it->second;
    }

    return location;
}
