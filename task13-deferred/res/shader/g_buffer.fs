#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

void main()
{    

    gPosition = FragPos;

    gNormal = normalize(Normal);

    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
}