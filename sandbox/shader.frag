#version 330 
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D noiseTex;

void main()
{
    vec3 noiseColor = texture(noiseTex, TexCoord).rgb;
    FragColor = vec4(noiseColor, 1.0);
}
