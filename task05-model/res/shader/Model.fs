#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform sampler2D texture_diffuse1;

void main()
{    
	vec3 Color = texture(texture_diffuse1, TexCoords).rgb;
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * Color;
  	
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * Color;
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * Color;  
        
    vec3 result = (ambient + diffuse + specular) * Color;
    FragColor = vec4(result, 1.0);

}