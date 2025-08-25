#version 330
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;
in float heightPercent;

uniform vec3 lightPos;

vec3 lightColor = vec3(1.0);

mat3 rotateY(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
         c, 0.0,  s,
         0.0, 1.0, 0.0,
        -s, 0.0,  c
    );
}

void main() {
    float widthPercent = clamp(TexCoords.x / 1.0, 0.0, 1.0);
    float normalMixFactor = widthPercent;
    vec3 baseColor = vec3 (0.05, 0.3, 0.01);
    vec3 tipColor = vec3 (0.5, 0.5, 0.1);

    vec3 diffuseColor = mix(baseColor, tipColor, heightPercent); 

    vec3 rotatedNormal1 = rotateY(3.14 * 0.3) * Normal;
    vec3 rotatedNormal2 = rotateY(3.14 * -0.3) * Normal;

    vec3 normal = mix(rotatedNormal1, rotatedNormal2, normalMixFactor);
    normal = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 grassColor = vec3(0.0, 0.86, 0.0);
    // ambient
    vec3 ambient = lightColor * vec3(0.5);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * diffuseColor;
    // FragColor = vec4(result, 1.0);
    FragColor = vec4(diffuseColor, 1.0);
}
