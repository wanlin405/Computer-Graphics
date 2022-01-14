/***
* ����  �������� (MAKE������ʱ��ɾ��ALL_BUILD��Ҳ���Խ�Task-sphere��ΪĬ����������)
* ����:
* 1-��ʼ��:       GLFW���ڣ�GLAD��
* 2-�������嶥�㣺ͨ����ѧ�������������ÿ����������
* 2-���ݴ���:     ͨ�����嶥�����깹���������������ɲ���VAO&VBO&EBO(׼����GPU�н��д���)�����ö�������ָ��(�����Ͼ��Ǹ���OpenGL��δ�������)��
* 3-��ɫ��:       ���������Ƭ����ɫ����Ȼ������Ϊ��ɫ��������Ⱦʱʹ����ɫ������
* 4-��Ⱦ:         ʹ�û���ģʽ��Բ���������޳����޳����棬ʹ���߿�ģʽ����
* 5-������        ��ջ��壬������������鴥���¼����ͷ���Դ
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>
#include <iostream>
#include <math.h>
#include <vector>
const unsigned int screen_width = 780;
const unsigned int screen_height = 780;

const GLfloat  PI = 3.14159265358979323846f;

//������ݻ��ֳ�50X50������
const int Y_SEGMENTS = 50;
const int X_SEGMENTS = 50;


int main()
{
	// ��ʼ��GLFW
	glfwInit();                                                     // ��ʼ��GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL�汾Ϊ3.3�����ΰ汾�ž���Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // ʹ�ú���ģʽ(������������)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // ���ʹ�õ���Mac OS Xϵͳ�����������
	glfwWindowHint(GLFW_RESIZABLE, false);						    // ���ɸı䴰�ڴ�С

																	// ��������(���ߡ���������)
	auto window = glfwCreateWindow(screen_width, screen_height, "Sphere", nullptr, nullptr);
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




	Shader shader("res/shader/task3.vs", "res/shader/task3.fs");//������ɫ��
	
	std::vector<float> sphereVertices;
	std::vector<int> sphereIndices;


	// ������Ķ���
	for (int y = 0; y <= Y_SEGMENTS; y++)
	{
		for (int x = 0; x <= X_SEGMENTS; x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);


			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
		}
	}

	// �������Indices
	for (int i = 0; i < Y_SEGMENTS; i++)
	{
		for (int j = 0; j < X_SEGMENTS; j++)
		{

			sphereIndices.push_back(i * (X_SEGMENTS+1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);

			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}


	// ��
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//���ɲ��������VAO��VBO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���������ݰ�����ǰĬ�ϵĻ�����
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	GLuint element_buffer_object; //EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// ���VAO��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// �����ɫ����
		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		//������
		//�������޳�(ֻ��Ҫչʾһ���棬��������غ�)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindVertexArray(VAO);
		//ʹ���߿�ģʽ����
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, X_SEGMENTS*Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
		//����ģʽ����
		//glPointSize(5);
		//glDrawElements(GL_POINTS, X_SEGMENTS*Y_SEGMENTS*6, GL_UNSIGNED_INT, 0);
		//�������岢�Ҽ���Ƿ��д����¼�(����������롢����ƶ��ȣ�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// ɾ��VAO��VBO��EBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &element_buffer_object);

	// �������е���Դ����ȷ�˳�����
	glfwTerminate();
	return 0;
}


