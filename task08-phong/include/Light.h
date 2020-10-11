#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Shader.h"

class Light
{
public:
    Light();
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    virtual ~Light();

    void SetAmbient(glm::vec3 ambient);
    void SetAmbient(float r, float g, float b);
    void SetDiffuse(glm::vec3 diffuse);
    void SetDiffuse(float r, float g, float b);
    void SetSpecular(glm::vec3 specular);
    void SetSpecular(float r, float g, float b);
    void SetOn(bool on);
    virtual void Draw(Shader shader, std::string name) = 0;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    bool on;
};

class DirectLight : public Light
{
public:
    DirectLight();
    DirectLight(glm::vec3 direction);
    DirectLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    ~DirectLight();

    void SetDirection(glm::vec3 direction);
    void SetDirection(float x, float y, float z);
    void Draw(Shader shader, std::string name);

    glm::vec3 direction;
};

class PointLight : public Light
{
public:
    PointLight();
    PointLight(glm::vec3 position);
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        float constant, float linear, float quadratic);
    ~PointLight();

    void SetPosition(glm::vec3 position);
    void SetPosition(float x, float y, float z);
    void SetAttenuationRatio(glm::vec3 attenuationRatio);
    void SetAttenuationRatio(float c, float l, float q);
    void Draw(Shader shader, std::string name);

    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

class SpotLight : public Light
{
public:
    SpotLight();
    SpotLight(glm::vec3 position, glm::vec3 direction);
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        float constant, float linear, float quadratic, float cut_off, float outer_cut_off);
    ~SpotLight();

    void SetPosition(glm::vec3 position);
    void SetPosition(float x, float y, float z);
    void SetDirection(glm::vec3 direction);
    void SetDirection(float x, float y, float z);
    void SetAttenuationRatio(glm::vec3 attenuation_ratio);
    void SetAttenuationRatio(float c, float l, float q);
    void SetIntensityRatio(glm::vec2 intensity_ratio);
    void SetIntensityRatio(float cut_off, float outer_cut_off);
    void Draw(Shader shader, std::string name);

    glm::vec3 position;
    glm::vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float cut_off;
    float outer_cut_off;
};

#endif