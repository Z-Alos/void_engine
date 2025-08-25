#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out float heightPercent;

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
};

void main() {
    heightPercent = clamp(aPos.y / 0.7, 0.0, 1.0);
    float curveAmount = randomLean * heightPercent;

    // Rotate around X-axis (forward/backward)
    mat3 grassMat = rotateX(curveAmount);
    vec3 bentPos = grassMat * aPos;

    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal * grassMat;
    gl_Position = projection * view * model * vec4(bentPos, 1.0);
}

