#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 aBoneIds; 
layout (location = 4) in vec4 aWeights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);
    
    // Check if vertex has any bone influences
    bool hasBoneInfluence = false;
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        if(aBoneIds[i] >= 0 && aWeights[i] > 0.0) {
            hasBoneInfluence = true;
            break;
        }
    }
    
    if(hasBoneInfluence) {
        // Apply bone transformations
        for(int i = 0; i < MAX_BONE_INFLUENCE; i++) {
            if(aBoneIds[i] == -1) 
                continue;
            if(aBoneIds[i] >= MAX_BONES) {
                totalPosition = vec4(aPos, 1.0f);
                break;
            }
            
            vec4 localPosition = finalBonesMatrices[aBoneIds[i]] * vec4(aPos, 1.0f);
            totalPosition += localPosition * aWeights[i];
            
            vec3 localNormal = mat3(finalBonesMatrices[aBoneIds[i]]) * aNormal;
            totalNormal += localNormal * aWeights[i];
        }
    } else {
        // No bone influence - use vertex as-is
        totalPosition = vec4(aPos, 1.0f);
        totalNormal = aNormal;
    }
    
    gl_Position = projection * view * model * totalPosition;
    
    FragPos = vec3(model * totalPosition);
    Normal = mat3(transpose(inverse(model))) * totalNormal;
    TexCoords = aTexCoords;
}
