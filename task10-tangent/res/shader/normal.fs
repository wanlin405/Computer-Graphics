#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoords;
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
    vec3 normal = texture(texture_normal, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
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