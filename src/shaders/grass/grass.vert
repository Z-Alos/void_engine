#version 330
layout (location = 0) in vec3 aPos;

uniform float randomLean;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

mat3 rotateX(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
        1.0, 0.0, 0.0,
        0.0,   c,  -s,
        0.0,   s,   c
    );
}

void main() {
    float heightPercent = clamp(aPos.y / 1.0, 0.0, 1.0);
    float curveAmount = randomLean * heightPercent;

    // Rotate around X-axis (forward/backward)
    mat3 grassMat = rotateX(curveAmount);
    vec3 bentPos = grassMat * aPos;

    gl_Position = projection * view * model * vec4(bentPos, 1.0);
}

