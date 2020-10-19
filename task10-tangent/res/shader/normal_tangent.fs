#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;


uniform sampler2D texture_material;
uniform sampler2D texture_normal;

uniform vec3 light_direction;
uniform vec3 light_ambient;
uniform vec3 light_diffuse;
uniform vec3 light_specular;

uniform vec3 view_position;

uniform mat4 model;

void main() {

// 从法线贴图范围[0,1]获取法线
    vec3 normal = texture(texture_normal, fs_in.TexCoords).rgb;
//// 将法线向量转换为范围[-1,1]
    normal = normalize(normal * 2.0f - 1.0f);
//引入切线到世界空间变换
    normal = normalize(fs_in.TBN * normal);
//// 像往常那样处理光照
    vec3 view_direction = normalize(view_position - fs_in.FragPos);

    vec3 light_direction = normalize(-light_direction);
    float diffuse_factor = max(dot(normal, light_direction), 0.0f);
    vec3 halfway = normalize(light_direction + view_direction);
    float spceular_factor = pow(max(dot(halfway, normal), 0.0f), 32);

    vec3 color = vec3(texture(texture_material, fs_in.TexCoords));
    vec3 ambient = color * light_ambient;
    vec3 diffuse = color * light_diffuse * diffuse_factor;
    vec3 specular = color * light_specular * spceular_factor;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}