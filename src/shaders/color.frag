#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
    // Settings
    float ambientStrength= 0.3f;
    float specularStrength = 1.0f;

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos); 
    vec3 reflectedDir = reflect(-lightDir, normal); 
    float spec = pow(max(dot(viewDir, reflectedDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}

