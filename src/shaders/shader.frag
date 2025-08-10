#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position; // not needed for directional light

    // vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

void main(){
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos); 
    vec3 reflectedDir = reflect(-lightDir, normal); 
    float spec = pow(max(dot(viewDir, reflectedDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    // Emission
    // vec3 emission = vec3(texture(material.emission, TexCoord));

    // Attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;   

    // vec3 result = ambient + diffuse + specular + emission;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}


