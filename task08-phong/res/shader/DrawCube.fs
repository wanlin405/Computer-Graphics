#version 330 core
out vec4 FragColor;

// 材质
struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;        // 镜面高光的散射/半径
}; 

// 定向光源
struct DirLight {
    bool on;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 点光源
struct PointLight {
    bool on;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float c;
    float l;
    float q;
};

// 聚光
struct SpotLight {
    bool on;
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float c;
    float l;
    float q;
};

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[6];
uniform SpotLight spotLight;

// 计算定向光
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if(!light.on) {
        return vec3(0.0);
    }
    // 漫反射
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 在漫反射光下物体颜色
    vec3 diffuseColor = vec3(material.diffuse);
    // 计算环境光，漫反射光和镜面光
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * vec3(material.specular);
    return ambient + diffuse + specular;
}

// 计算点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if(!light.on) {
        return vec3(0.0);
    }
    // 漫反射
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 距离和衰减
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.c + light.l * d + light.q * d * d);
    // 在漫反射光下物体颜色
    vec3 diffuseColor = vec3(material.diffuse);
    // 计算环境光，漫反射光和镜面光
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * vec3(material.specular);
    return (ambient + diffuse + specular) * attenuation;
}

// 计算聚光
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if(!light.on) {
        return vec3(0.0);
    }
    // 漫反射
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0); 
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 距离和衰减
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.c + light.l * d + light.q * d * d);
    // 聚光强度
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 在漫反射光下物体颜色
    vec3 diffuseColor = vec3(material.diffuse);
    // 计算环境光，漫反射光和镜面光
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = diff * light.diffuse * diffuseColor;
    vec3 specular = vec3(material.specular) * spec * light.specular;
    // 乘聚光强度为了避免光照过强，平滑聚光边缘
    return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // 计算定向光
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 计算点光源
    for (int i = 0; i < 6; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // 计算聚光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}