#version 330 core
out vec4 color;
in vec2 TexCoords;

uniform sampler2D shadowMap;

void main()
{             
    float depthValue = texture(shadowMap, TexCoords).r;
    color = vec4(vec3(depthValue), 1.0); 
}
