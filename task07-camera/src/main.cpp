/***
 * ����  ����������� (MAKE������ʱ��ɾ��ALL_BUILD��Ҳ���Խ�Task-camera��ΪĬ����������)
 * ����:
 * �������������в�������    
 */

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Camera.h"

//��������Ӧ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

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
//�������ز���

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // ��ʼ��GLFW
    glfwInit();                                                     // ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL�汾Ϊ3.3�����ΰ汾�ž���Ϊ3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ(������������)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // ���ʹ�õ���Mac OS Xϵͳ�����������
    glfwWindowHint(GLFW_RESIZABLE, false);						    // ���ɸı䴰�ڴ�С

    // ��������(���ߡ���������)
    auto window = glfwCreateWindow(screen_width, screen_height, "Camera", nullptr, nullptr);
    if (window == nullptr) {                                     
        std::cout << "Failed to Create OpenGL Context" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ��ʼ��GLAD������OpenGL����ָ���ַ�ĺ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ָ����ǰ�ӿڳߴ�(ǰ��������Ϊ���½�λ�ã���������������Ⱦ���ڿ���)
    glViewport(0, 0, screen_width, screen_height);
	Shader shader("res/shader/task-camera.vs", "res/shader/task-camera.fs");//������ɫ��
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

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window)) {

		//����ÿ֡��ʱ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������ɫ�������Ȼ���

		shader.Use();

		// Transform����任����
		glm::mat4 model(1);//model���󣬾ֲ�����任����������
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 view(1);//view������������任���۲�����ϵ
		view = camera.GetViewMatrix();
		glm::mat4 projection(1);//projection����ͶӰ����
		projection = glm::perspective(glm::radians(camera.Zoom), screen_width / screen_height, 0.1f, 100.0f);

		// ����ɫ���д������
		shader.SetMat4("model",model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

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

//��������
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

//���ڷֱ�������Ӧ
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//����ƶ���Ӧ
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
//��������Ӧ
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}




