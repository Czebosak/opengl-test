#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_mpv;

void main() {
    gl_Position = u_mpv * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main() {
    color = u_color;
};
