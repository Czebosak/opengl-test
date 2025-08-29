#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 mvp;
//uniform vec2 scale;

void main() {
    //gl_Position = mvp * (vec4(scale, 0.0f, 1.0f) * position);
    gl_Position = mvp * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main() {
    color = u_color;
};
