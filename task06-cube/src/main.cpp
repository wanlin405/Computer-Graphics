//��������
//1. ��ʼ������������
//2. ���������嶥��VAOVBO�Լ���ɫ����������Ȳ���
//3. ������ѭ���������
//4. ʹ����������ɫ�������첢����pvm���󣬻���
//5. ѭ���������ͷ�VAOVBO

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"

const float vertices[] = {                  //����������
	-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f,0.0f,0.0f,
	-0.5f, -0.5f, -0.5f,  1.0f,0.0f,0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,1.0f,0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,1.0f,0.0f,

	-0.5f,  0.5f,  0.5f,  0.0f,0.0f,1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,0.0f,1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,0.0f,1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,0.0f,1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,0.0f,1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,0.0f,1.0f,

	0.5f,  0.5f,  0.5f,  0.5f,0.0f,0.0f,
	0.5f,  0.5f, -0.5f,  0.5f,0.0f,0.0f,
	0.5f, -0.5f, -0.5f,  0.5f,0.0f,0.0f,
	0.5f, -0.5f, -0.5f,  0.5f,0.0f,0.0f,
	0.5f, -0.5f,  0.5f,  0.5f,0.0f,0.0f,
	0.5f,  0.5f,  0.5f,  0.5f,0.0f,0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f,0.5f,0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,0.5f,0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,0.5f,0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,0.5f,0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,0.5f,0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,0.5f,0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,0.0f,0.5f,
	0.5f,  0.5f, -0.5f,  0.0f,0.0f,0.5f,
	0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.5f,
	0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.5f,
	-0.5f,  0.5f,  0.5f,  0.0f,0.0f,0.5f,
	-0.5f,  0.5f, -0.5f,  0.0f,0.0f,0.5f
};

float screen_width = 1280.0f;          //���ڿ��
float screen_height = 720.0f;          //���ڸ߶�
//�������
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 3.0f);     //�����λ��
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);       //���������
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);           //�����������
//��Ұ
float fov = 45.0f;

int main() {
	// ��ʼ��GLFW
	glfwInit();                                                     // ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL�汾Ϊ3.3�����ΰ汾�ž���Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ(������������)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // ���ʹ�õ���Mac OS Xϵͳ�����������
	glfwWindowHint(GLFW_RESIZABLE, false);						    // ���ɸı䴰�ڴ�С

																	// ��������(���ߡ���������)
	auto window = glfwCreateWindow(screen_width, screen_height, "Cube", nullptr, nullptr);
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


	Shader shader("res/shader/task-cube.vs", "res/shader/task-cube.fs");//������ɫ��

	// ���ɲ���VAO��VBO
	GLuint vertex_array_object; // == VAO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	GLuint vertex_buffer_object; // == VBO
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// ���������ݰ�����ǰĬ�ϵĻ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glEnable(GL_DEPTH_TEST);
	// Render loop��ѭ��
	while (!glfwWindowShouldClose(window)) {
		//������ѭ����������ɫ������Ȼ���
		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������ɫ�������Ȼ���

		shader.Use();

		// Transform����任����
		glm::mat4 model(1);//model���󣬾ֲ�����任����������
		model = glm::translate(model, glm::vec3(0.0,0.0,0.0));
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f,1.0f,1.0f));
		glm::mat4 view(1);//view������������任���۲�����ϵ
		view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
		glm::mat4 projection(1);//projection����ͶӰ����
		projection = glm::perspective(glm::radians(fov), (float)screen_width / screen_height, 0.1f, 100.0f);

		
		// ����ɫ���д������
		int model_location = glGetUniformLocation(shader.ID, "model"); //��ȡ��ɫ����ĳ��������λ��
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));//д�����ֵ
		int view_location = glGetUniformLocation(shader.ID, "view");
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		int projection_location = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
		//����
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//�ͷ�VAOVBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);

	// �������е���Դ����ȷ�˳�����
	glfwTerminate();
	return 0;
}