#include <GL/glew.h>
#include <alloca.h>
#include <fstream>
#include <sstream>
#include <string>

#include "renderer.h"

struct ShaderProgramSource {
    std::string vertex_source;
    std::string fragment_source;
};

ShaderProgramSource parse_shader(const std::string& filename);

unsigned int compile_shader(unsigned int type, const std::string& source);

unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
