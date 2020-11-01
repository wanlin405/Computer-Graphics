#include "Shader.h"
#include "fstream"
#include "sstream"
#include "iostream"

Shader::Shader(const GLchar* vertex_shader_path, const GLchar* fragment_shader_path)
{
    std::string vertex_shader_code;
    std::string fragment_shader_code;
    if (GetShaderFromFile(vertex_shader_path, fragment_shader_path, &vertex_shader_code, &fragment_shader_code))
    {
        return;
    }
    if (LinkShader(vertex_shader_code.c_str(), fragment_shader_code.c_str()))
    {
        return;
    }
}

Shader::~Shader()
{

}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const
{
    SetInt(name, (int)value);
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(GetUniform(name), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(GetUniform(name), value);
}

void Shader::SetVec2(const std::string &name, float x, float y) const
{
    glUniform2f(GetUniform(name), x, y);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{
    SetVec2(name, value.x, value.y);
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(GetUniform(name), x, y, z);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    SetVec3(name, value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(GetUniform(name), x, y, z, w);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
    SetVec4(name, value.x, value.y, value.z, value.w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &value) const
{
    glUniformMatrix2fv(GetUniform(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &value) const
{
    glUniformMatrix3fv(GetUniform(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, &value[0][0]);
}

int Shader::GetShaderFromFile(const GLchar* vertex_shader_path, const GLchar* fragment_shader_path, std::string *vertex_shader_code, std::string *fragment_shader_code)
{
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;
    vertex_shader_file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    fragment_shader_file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try
    {
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);
        std::stringstream vertex_shader_stream, fragment_shader_stream;
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();
        vertex_shader_file.close();
        fragment_shader_file.close();
        *vertex_shader_code = vertex_shader_stream.str();
        *fragment_shader_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Load Shader File Error!" << std::endl;
        return -1;
    }
    return 0;
}

int Shader::LinkShader(const char* vertex_shader_code, const char* fragment_shader_code)
{
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    CheckCompileErrors(vertex_shader, "VERTEX");

    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    CheckCompileErrors(fragment_shader, "FRAGMENT");

    this->ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return 0;
}

int Shader::GetUniform(const std::string &name) const
{
    int position = glGetUniformLocation(ID, name.c_str());
    if (position == -1)
    {
        std::cout << "uniform " << name << " set failed!" << std::endl;
    }
    return position;
}

void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[512];
    if (type == "PROGRAM")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR!\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
}