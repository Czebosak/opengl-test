#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 VERTEX_UV;

out vec2 UV;

void main() {
    gl_Position = position;
    UV = VERTEX_UV;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 UV;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
    vec4 sample = texture(u_texture, UV);
    color = sample * u_color;
};
