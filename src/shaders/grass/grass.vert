#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;
layout (location = 7) in float randomLean;  

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out float heightPercent;

uniform float time;
uniform mat4 view;
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

    vec3 windDirection = vec3(1.0, 0.0, 0.0);
    heightPercent = clamp(aPos.y / 0.8, 0.0, 1.0);
    
    vec3 worldPos = vec3(model * vec4(aPos, 1.0));
    
    float baseLean = randomLean * heightPercent;
    
    // Sway
    float swayStrength = heightPercent * 0.11;
    float windSway = sin(time * 2.0 + worldPos.x * 0.5 + worldPos.z * 0.3) * swayStrength;
    
    float totalCurve = baseLean + windSway;
    
    // Apply rotation
    mat3 grassMat = rotateX(totalCurve);
    vec3 bentPos = grassMat * aPos;
    bentPos += windDirection * totalCurve * heightPercent;
    vec3 bentNormal = grassMat * aNormal;
    
    // Output
    FragPos = vec3(model * vec4(bentPos, 1.0));
    TexCoords = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * bentNormal;
    // Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * model * vec4(bentPos, 1.0);
}
