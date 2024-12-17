#include "shader_loader.h"

ShaderProgramSource parse_shader(const std::string& filename) {
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

unsigned int compile_shader(unsigned int type, const std::string& source) {
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

unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader) {
    gl_call(unsigned int program = glCreateProgram());
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
