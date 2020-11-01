#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform float num;

struct Light {
    vec3 Position;
    vec3 Color;
};
const int NR_LIGHTS = 50;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{             
    // 从G-buffer中提取数据
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = 1.0f;
    
    // 光照计算
    vec3 lighting  = Diffuse * 0.9; 
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
		//环境光
		vec3 ambient = Diffuse * 0.01;
        // 漫反射
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
        // 镜面高光
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].Color * spec * Specular;

        lighting += ambient + diffuse + specular;        
    }
    FragColor = vec4(lighting/num, 1.0);
}