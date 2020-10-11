#include "Light.h"

glm::vec3 DEFAULT_AMBIENT = glm::vec3(0.2f);
glm::vec3 DEFAULT_DIFFUSE = glm::vec3(0.5f);
glm::vec3 DEFAULT_SPECULAR = glm::vec3(1.0f);
glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f);
glm::vec3 DEFAULT_DIRECTION = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 DEFAULT_ATTENUATION_RATIO = glm::vec3(1.0f, 0.09f, 0.032f);
glm::vec2 DEFAULT_INTENSITY_RATIO = glm::vec2(cos(glm::radians(12.5f)), cos(glm::radians(15.0f)));

Light::Light()
: Light(DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR)
{
}

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Light::~Light()
{
}

void Light::SetAmbient(glm::vec3 ambient)
{
    this->ambient = ambient;
}

void Light::SetAmbient(float r, float g, float b)
{
    SetAmbient(glm::vec3(r, g, b));
}

void Light::SetDiffuse(glm::vec3 diffuse)
{
    this->diffuse = diffuse;
}

void Light::SetDiffuse(float r, float g, float b)
{
    SetDiffuse(glm::vec3(r, g, b));
}

void Light::SetSpecular(glm::vec3 specular)
{
    this->specular = specular;
}

void Light::SetSpecular(float r, float g, float b)
{
    SetSpecular(glm::vec3(r, g, b));
}

void Light::SetOn(bool on)
{
    this->on = on;
}

DirectLight::DirectLight()
: DirectLight(DEFAULT_DIRECTION)
{
}

DirectLight::DirectLight(glm::vec3 direction)
: DirectLight(direction, DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR)
{
}

DirectLight::DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
: Light(ambient, diffuse, specular)
{
    this->direction = direction;
}

DirectLight::~DirectLight()
{
}

void DirectLight::SetDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void DirectLight::SetDirection(float x, float y, float z)
{
    SetDirection(glm::vec3(x, y, z));
} 

void DirectLight::Draw(Shader shader, std::string name)
{
    //shader.SetBool(name + ".on", on);
    shader.SetVec3(name + ".direction", direction);
    shader.SetVec3(name + ".ambient", ambient);
    shader.SetVec3(name + ".diffuse", diffuse);
    shader.SetVec3(name + ".specular", specular);
}

PointLight::PointLight()
: PointLight(DEFAULT_POSITION)
{
}

PointLight::PointLight(glm::vec3 position)
: PointLight(position, DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR)
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
: PointLight(position, ambient, diffuse, specular, DEFAULT_ATTENUATION_RATIO.x, DEFAULT_ATTENUATION_RATIO.y, DEFAULT_ATTENUATION_RATIO.z)
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
: Light(ambient, diffuse, specular)
{
    this->position = position;
    this->constant = constant;
    this->linear = constant;
    this->quadratic = quadratic;
}

PointLight::~PointLight()
{
}

void PointLight::SetPosition(glm::vec3 position)
{
    this->position = position;
}

void PointLight::SetPosition(float x, float y, float z)
{
    SetPosition(glm::vec3(x, y, z));
}

void PointLight::SetAttenuationRatio(float c, float l, float q)
{
    this->constant = c;
    this->linear = l;
    this->quadratic = q;
}

void PointLight::SetAttenuationRatio(glm::vec3 attenuation_ratio)
{
    SetAttenuationRatio(attenuation_ratio.x, attenuation_ratio.y, attenuation_ratio.z);
}

void PointLight::Draw(Shader shader, std::string name)
{
    shader.SetBool(name + ".on", on);
    shader.SetVec3(name + ".position", position);
    shader.SetVec3(name + ".ambient", ambient);
    shader.SetVec3(name + ".diffuse", diffuse);
    shader.SetVec3(name + ".specular", specular);
    shader.SetFloat(name + ".c", constant);
    shader.SetFloat(name + ".l", linear);
    shader.SetFloat(name + ".q", quadratic);
}

SpotLight::SpotLight()
: SpotLight(DEFAULT_POSITION, DEFAULT_DIRECTION)
{
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction)
: SpotLight(position, direction, DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR)
{
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
: SpotLight(position, direction, ambient, diffuse, specular, DEFAULT_ATTENUATION_RATIO.x, DEFAULT_ATTENUATION_RATIO.y,
DEFAULT_ATTENUATION_RATIO.z, DEFAULT_INTENSITY_RATIO.x, DEFAULT_INTENSITY_RATIO.y)
{
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
: Light(ambient, diffuse, specular)
{
    this->position = position;
    this->direction = direction;
    this->constant = constant;
    this->linear = constant;
    this->quadratic = quadratic;
    this->cut_off = cutOff;
    this->outer_cut_off = outerCutOff;
}

SpotLight::~SpotLight()
{
}

void SpotLight::SetPosition(glm::vec3 position)
{
    this->position = position;
}

void SpotLight::SetPosition(float x, float y, float z)
{
    SetPosition(glm::vec3(x, y, z));
}

void SpotLight::SetDirection(glm::vec3 direction)
{
    this->direction = direction;
}

void SpotLight::SetDirection(float x, float y, float z)
{
    SetDirection(glm::vec3(x, y, z));
}

void SpotLight::SetAttenuationRatio(float c, float l, float q)
{
    this->constant = c;
    this->linear = l;
    this->quadratic = q;
}

void SpotLight::SetAttenuationRatio(glm::vec3 attenuation_ratio)
{
    SetAttenuationRatio(attenuation_ratio.x, attenuation_ratio.y, attenuation_ratio.z);
}

void SpotLight::SetIntensityRatio(float cutOff, float outerCutOff)
{
    this->cut_off = cutOff;
    this->outer_cut_off = outerCutOff;
}

void SpotLight::SetIntensityRatio(glm::vec2 intensity_ratio)
{
    SetIntensityRatio(intensity_ratio.x, intensity_ratio.y);
}

void SpotLight::Draw(Shader shader, std::string name)
{
    shader.SetBool(name + ".on", on);
    shader.SetVec3(name + ".position", position);
    shader.SetVec3(name + ".direction", direction);
    shader.SetVec3(name + ".ambient", ambient);
    shader.SetVec3(name + ".diffuse", diffuse);
    shader.SetVec3(name + ".specular", specular);
    shader.SetFloat(name + ".c", constant);
    shader.SetFloat(name + ".l", linear);
    shader.SetFloat(name + ".q", quadratic);
    shader.SetFloat(name + ".cutOff", cut_off);
    shader.SetFloat(name + ".outerCutOff", outer_cut_off);
}