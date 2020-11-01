/***
* Task 粒子系统 1
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/Shader.h"
#include "../include/Camera.h"
#include "../include/Texture.h"

#include <iostream>
#include <string>
#include <algorithm> //排序
using namespace std;

// 窗口大小调整的回调函数(当窗口大小改变时，视口也要改变)
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 鼠标控制回调
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// 滚轮控制回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 键盘控制回调
void processInput(GLFWwindow *window);

// 屏幕宽，高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); //摄像机位置

float lastX = (float)SCR_WIDTH / 2.0, lastY = (float)SCR_HEIGHT / 2.0; // 设置鼠标初始位置为屏幕中心
bool firstMouse = true;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 粒子
struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // 颜色
	float size; //粒子大小
	float life; // 粒子的剩余生命，小于0表示消亡.
	float cameradistance; // 粒子和摄像头的距离

	bool operator<(const Particle& that) const {
		// 逆序排序， 远的粒子排在前面
		return this->cameradistance > that.cameradistance;
	}
};

const int MaxParticles = 200; //最大粒子数
const float life = 2.0; //粒子的存活时间
glm::vec3 startPos = glm::vec3(0.0f, 0, 0.0f); //粒子起点
glm::vec3 endPos = glm::vec3(0.0f, 0, 4.8f); //粒子终点
Particle ParticlesContainer[MaxParticles];

int LastUsedParticle = 0;
// 在粒子数组中，找到生命消亡的粒子
int FindUnusedParticle() {
	for (int i = LastUsedParticle; i<MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}
	for (int i = 0; i<LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}
	return 0;
}

// 根据cameradistance给粒子排序
void SortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}

int main()
{
	// ---------------------初始化--------------------------
	// glfw初始化，采用的GL版本为3.3核心版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建GL窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "particle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 指定上下文为当前窗口
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏光标，鼠标停留在窗口内

	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	static GLfloat* particle_position_size_data = new GLfloat[5000];
	static GLubyte* particle_color_data = new GLubyte[5000];
	for (int i = 0; i<MaxParticles; i++) 
	{
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	GLuint particleTexture = Texture::LoadTextureFromFile("res/texture/circle.png");

	// 粒子顶点位置
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	//  粒子的顶点坐标 （每个粒子都一样）
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// 着色器
	Shader shader("res/shader/particle_system.vs", "res/shader/particle_system.fs");

	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		// 在每一帧中计算出新的deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// 输入控制
		processInput(window);

		// 清除颜色和深度缓冲
		glClearColor(0.0f, 57.0f / 255, 92.0f / 255, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 用于传给顶点着色器，利用公告板技术绘制粒子
		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 ViewMatrix = camera.GetViewMatrix();
		glm::vec3 CameraPosition = camera.Position;
		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

		// 消亡多少粒子，产生多少粒子
		int newparticles = deltaTime / life * MaxParticles;

		for (int i = 0; i<newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = life;
			glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f); //主要方向
			//产生随机的位置偏差
			glm::vec3 randomdOffset = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1500.0f, //[-0.333, 0.333]
				(rand() % 2000 - 1000.0f) / 1500.0f,
				(rand() % 2000 - 1000.0f) / 1500.0f
			);
			ParticlesContainer[particleIndex].pos = startPos + randomdOffset; //粒子起点
			ParticlesContainer[particleIndex].speed = (endPos - startPos) / life;

			// 产生随机的颜色、透明度、大小
			ParticlesContainer[particleIndex].r = rand() % 256; 
			ParticlesContainer[particleIndex].g = rand() % 256;
			ParticlesContainer[particleIndex].b = rand() % 256;
			ParticlesContainer[particleIndex].a = (rand() % 100) + 50;
			ParticlesContainer[particleIndex].size = (rand() % 1000) / 50000 + 0.04f; //[0.04, 0.06]
		}

		// 模拟所有的粒子
		int ParticlesCount = 0;
		for (int i = 0; i<MaxParticles; i++) {
			Particle& p = ParticlesContainer[i]; // 引用
			if (p.life > 0.0f) {
				p.life -= deltaTime;
				if (p.life > 0.0f) {
					p.pos += p.speed * (float)deltaTime;
					p.cameradistance = glm::length(p.pos - CameraPosition);
					//填充数据
					particle_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					particle_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					particle_position_size_data[4 * ParticlesCount + 2] = p.pos.z;
					particle_position_size_data[4 * ParticlesCount + 3] = p.size;
					particle_color_data[4 * ParticlesCount + 0] = p.r;
					particle_color_data[4 * ParticlesCount + 1] = p.g;
					particle_color_data[4 * ParticlesCount + 2] = p.b;
					particle_color_data[4 * ParticlesCount + 3] = p.a;
				}
				else {
					//已经消亡的粒子，在调用SortParticles()之后，会被放在数组的最后
					p.cameradistance = -1.0f;
				}
				ParticlesCount++;
			}
		}

		SortParticles();

		//开启混合
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader.Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, particleTexture);
		shader.SetInt("myTextureSampler", 0);

		// 摄像头的右方向
		shader.SetVec3("CameraRight_worldspace", ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		// 摄像头的上方向
		shader.SetVec3("CameraUp_worldspace", ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
		shader.SetMat4("VP", ViewProjectionMatrix);

		glBindVertexArray(VertexArrayID);
		for (int i = 0; i < ParticlesCount; i++)
		{
			shader.SetVec4("xyzs", particle_position_size_data[4 * i + 0],
				particle_position_size_data[4 * i + 1],
				particle_position_size_data[4 * i + 2],
				particle_position_size_data[4 * i + 3]);
			shader.SetVec4("color", particle_color_data[4 * i + 0],
				particle_color_data[4 * i + 1],
				particle_color_data[4 * i + 2],
				particle_color_data[4 * i + 3]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete[] particle_position_size_data;

	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteProgram(shader.ID);
	glDeleteTextures(1, &particleTexture);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();
	return 0;
}

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

