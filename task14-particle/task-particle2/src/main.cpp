/***
* Task 粒子系统
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
	float size, angle, weight;
	float life; // 粒子的剩余生命，小于0表示消亡.
	float cameradistance; // *Squared* 距离摄像头的具体， 如果 dead : -1.0f

	bool operator<(const Particle& that) const {
		// 逆序排序， 远的粒子排在前面
		return this->cameradistance > that.cameradistance;
	}
};

const int MaxParticles = 1000; //最大粒子数
const float spread = 3.0f; //粒子扩散程度
const float life = 5.0; //粒子的存活时间
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

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static GLfloat* g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	static GLubyte* g_particule_color_data = new GLubyte[MaxParticles * 4];
	for (int i = 0; i<MaxParticles; i++) 
	{
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	GLuint particleTexture = Texture::LoadTextureFromFile("res/texture/xuehua.png");

	// 粒子顶点位置
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	//  粒子的顶点坐标 （每个粒子都一样）
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//  粒子的位置和大小
	GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);//初始化为NULL，后续根据粒子的属性，进行填充

	//  包含了粒子的 颜色
	GLuint particles_color_buffer;
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);//初始化为NULL，后续根据粒子的属性，进行填充

																							// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

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

		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 ViewMatrix = camera.GetViewMatrix();
		glm::vec3 CameraPosition = camera.Position;
		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

		// 消亡多少粒子，产生多少粒子
		int newparticles = deltaTime / life * MaxParticles;

		for (int i = 0; i<newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = life;
			ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f); //粒子起点
			glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f); //主要方向
			//产生随机的方向偏差
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f, //[-1,1]
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);

			ParticlesContainer[particleIndex].speed = maindir + randomdir * spread;

			// 产生随机的颜色、透明度、大小
			ParticlesContainer[particleIndex].r = rand() % 256; 
			ParticlesContainer[particleIndex].g = rand() % 256;
			ParticlesContainer[particleIndex].b = rand() % 256;
			ParticlesContainer[particleIndex].a = (rand() % 100) + 50;
			ParticlesContainer[particleIndex].size = (rand() % 1000) / 5000.0f + 0.6f; //[0.6, 0.8]
		}

		// 模拟所有的粒子
		int ParticlesCount = 0;
		for (int i = 0; i<MaxParticles; i++) {
			Particle& p = ParticlesContainer[i]; // 引用
			if (p.life > 0.0f) {
				p.life -= deltaTime;
				if (p.life > 0.0f) {
					// 模拟简单物理效果：只有重力，没有碰撞
					p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)deltaTime * 0.5f;
					p.pos += p.speed * (float)deltaTime;
					p.cameradistance = glm::length(p.pos - CameraPosition);
					//填充数据
					g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;
					g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;
					g_particule_color_data[4 * ParticlesCount + 0] = p.r;
					g_particule_color_data[4 * ParticlesCount + 1] = p.g;
					g_particule_color_data[4 * ParticlesCount + 2] = p.b;
					g_particule_color_data[4 * ParticlesCount + 3] = p.a;
				}
				else {
					//已经消亡的粒子，在调用SortParticles()之后，会被放在数组的最后
					p.cameradistance = -1.0f;
				}
				ParticlesCount++;
			}
		}

		SortParticles();

		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

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

		// 粒子的顶点坐标
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,                  
			3,                
			GL_FLOAT,         
			GL_FALSE,         
			0,                
			(void*)0            
		);

		// 粒子的中心位置
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,                                
			4,                                // size : x + y + z + size = 4
			GL_FLOAT,                         
			GL_FALSE,                         
			0,                               
			(void*)0                          
		);

		// 粒子的颜色
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,                                 
			4,                                // size : r + g + b + a = 4
			GL_UNSIGNED_BYTE,                 
			GL_TRUE,                          
			0,                                
			(void*)0                          
		);

		glVertexAttribDivisor(0, 0); // 粒子顶点坐标 : 总是重用相同的 4 个顶点坐标，所以第二个参数是 0
		glVertexAttribDivisor(1, 1); // 粒子的中心位置和大小，每一个粒子不同，所以第二个参数是 1
		glVertexAttribDivisor(2, 1); // 粒子的颜色，每一个粒子不同，所以第二个参数是 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete[] g_particule_position_size_data;

	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
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

