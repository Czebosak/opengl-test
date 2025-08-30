#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in ivec2 osu_position;
layout(location = 3) in float approach;

out vec2 uv;
out float frag_approach;
out float screen_radius;    

uniform float circle_diameter;
uniform mat4 mvp;

void main() {
    vec2 scaled = position * circle_diameter * (1.0f + (2.0f * approach));
    gl_Position = mvp * vec4(scaled + vec2(osu_position), 0.0f, 1.0f);

    uv = vertex_uv;
    frag_approach = approach;
    screen_radius = length(scaled);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv;
in float frag_approach;
in float screen_radius;

const float THICKNESS_PIXELS = 5.0f;

void main() {
    vec2 center = vec2(0.5f, 0.5f);
    float radius = 0.5f;

    float thickness_uv = THICKNESS_PIXELS / (screen_radius * 2.0);
    float inner_circle_radius = radius - thickness_uv;

    float dist = distance(uv, center);

    if (dist > radius || dist < inner_circle_radius) {
        discard;
    } else {
        color = vec4(1.0f);
    }
};
