/***
 * ����  �����ı��� (MAKE������ʱ��ɾ��ALL_BUILD��Ҳ���Խ�Task-quad��ΪĬ����������)
 * ����:
 * 1-��ʼ��:   GLFW���ڣ�GLAD��
 * 2-���ݴ���: �����������ݣ����ɲ���VAO&VBO(׼����GPU�н��д���)�����ö�������ָ��(�����Ͼ��Ǹ���OpenGL��δ�������)��
 * 3-��ɫ��:   ���������Ƭ����ɫ����Ȼ������Ϊ��ɫ��������Ⱦʱʹ����ɫ������
 * 4-��Ⱦ:     ��ջ��壬ʹ����ɫ�����򣬻����ı��Σ�������������鴥���¼����ͷ���Դ        
 */

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// �ı��εĶ�������
float vertices[] = {
	// ��һ��������
	0.5f, 0.5f, 0.0f,     // ����
	0.5f, -0.5f, 0.0f,    // ����
	-0.5f, -0.5f, 0.0f,   // ����
	// �ڶ���������
	-0.5f, -0.5f, 0.0f,   // ����
	0.5f, 0.5f, 0.0f,     // ����
	-0.5f, 0.5f, 0.0f     // ����
};

// ��������(ע�������Ǵ�0��ʼ��)
unsigned int indices[] = {
	0, 1, 5,              // ��һ��������
	1, 2, 5               // �ڶ���������
};

// ��Ļ����
int screen_width = 1280;
int screen_height = 720;

int main() {
    // ��ʼ��GLFW
    glfwInit();                                                     // ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL�汾Ϊ3.3�����ΰ汾�ž���Ϊ3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ(������������)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // ���ʹ�õ���Mac OS Xϵͳ�����������
    glfwWindowHint(GLFW_RESIZABLE, false);						    // ���ɸı䴰�ڴ�С

    // ��������(���ߡ���������)
    auto window = glfwCreateWindow(screen_width, screen_height, "Quad", nullptr, nullptr);
    if (window == nullptr) {                                        // ������ڴ���ʧ�ܣ����Failed to Create OpenGL Context
        std::cout << "Failed to Create OpenGL Context" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);                                 // �����ڵ�����������Ϊ��ǰ�̵߳���������

    // ��ʼ��GLAD������OpenGL����ָ���ַ�ĺ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ָ����ǰ�ӿڳߴ�(ǰ��������Ϊ���½�λ�ã���������������Ⱦ���ڿ���)
    glViewport(0, 0, screen_width, screen_height);

    // ���ɲ���VAO��VBO
    GLuint vertex_array_object; // == VAO
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint vertex_buffer_object; // == VBO
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    // ���������ݰ�����ǰĬ�ϵĻ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint element_buffer_object; // == EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ���VAO��VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // ������ɫ����Ƭ����ɫ��Դ��
    const char *vertex_shader_source =
        "#version 330 core\n"                           
        "layout (location = 0) in vec3 aPos;\n"           // λ�ñ���������λ��ֵΪ0
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}\n\0";
    const char *fragment_shader_source =
        "#version 330 core\n"
        "out vec4 FragColor;\n"                           // �������ɫ����
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    // ���ɲ�������ɫ��
    // ������ɫ��
    int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader); 
    int success;
    char info_log[512];
    // �����ɫ���Ƿ�ɹ����룬�������ʧ�ܣ���ӡ������Ϣ
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }
    // Ƭ����ɫ��
    int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    // �����ɫ���Ƿ�ɹ����룬�������ʧ�ܣ���ӡ������Ϣ
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }
    // ���Ӷ����Ƭ����ɫ����һ����ɫ������
    int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // �����ɫ���Ƿ�ɹ����ӣ��������ʧ�ܣ���ӡ������Ϣ
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
	// ɾ����ɫ��
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

	// �߿�ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window)) {

		// �����ɫ����
		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// ʹ����ɫ������
        glUseProgram(shader_program);

		// �����ı���
        glBindVertexArray(vertex_array_object);                                    // ��VAO
		// �����ı���
        glDrawArrays(GL_TRIANGLES, 0, 6);                                          
		// ��EBO�����ı���
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);                                                      // �����

		// �������岢�Ҽ���Ƿ��д����¼�(����������롢����ƶ��ȣ�
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	// ɾ��VAO/VBO/EBO
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
	glDeleteBuffers(1, &element_buffer_object);

	// �������е���Դ����ȷ�˳�����
    glfwTerminate();
    return 0;
}