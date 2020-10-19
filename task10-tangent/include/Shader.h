#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "string"

class Shader
{
public:
    unsigned int ID;

    Shader(const GLchar* vertex_shader_path, const GLchar* fragment_shader_path);
    ~Shader();

    void Use();
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec2(const std::string &name, float x, float y) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;
    void SetVec4(const std::string &name, const glm::vec4 &value) const;
    void SetVec4(const std::string &name, float x, float y, float z, float w) const;
    void SetMat2(const std::string &name, const glm::mat2 &value) const;
    void SetMat3(const std::string &name, const glm::mat3 &value) const;
    void SetMat4(const std::string &name, const glm::mat4 &value) const;

private:
    int GetShaderFromFile(const GLchar* vertex_shader_path, const GLchar* fragment_shader_path,
        std::string *vertex_shader_code, std::string *fragment_shader_code);
    int LinkShader(const char* vertex_shader_code, const char* fragment_shader_code);
    int GetUniform(const std::string &name) const;
    void CheckCompileErrors(GLuint shader, std::string type);
};

#endif // !__SHADER_H__
