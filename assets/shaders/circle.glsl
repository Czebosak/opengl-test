#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in ivec2 osu_position;

out vec2 uv;

uniform float circle_diameter;
uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4((position * circle_diameter) + vec2(osu_position), 0.0f, 1.0f);
    uv = vertex_uv;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 uv;

const float INNER_CIRCLE_RADIUS = 0.9f;

void main() {
    vec2 center = vec2(0.5f, 0.5f);
    float radius = 0.5f;
    float dist = distance(uv, center);

    if (dist > radius) {
        discard;
    } else if (dist < radius * INNER_CIRCLE_RADIUS) {
        color = vec4(0.2f, 0.2f, 0.2f, dist / radius * INNER_CIRCLE_RADIUS);
    } else {
        color = vec4(1.0f);
    }
};
