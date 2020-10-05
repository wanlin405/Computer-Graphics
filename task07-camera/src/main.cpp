/***
 * 例程  摄像机类的添加 (MAKE后运行时可删除ALL_BUILD，也可以将Task-camera设为默认启动工程)
 * 步骤:
 * 加载摄像机类进行操作即可    
 */

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include "Camera.h"

//鼠标键盘响应函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const float vertices[] = {                  //立方体数组
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

float screen_width = 1280.0f;          //窗口宽度
float screen_height = 720.0f;          //窗口高度
//摄像机相关参数

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    // 初始化GLFW
    glfwInit();                                                     // 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL版本为3.3，主次版本号均设为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式(无需向后兼容性)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 如果使用的是Mac OS X系统，需加上这行
    glfwWindowHint(GLFW_RESIZABLE, FALSE);						    // 不可改变窗口大小

    // 创建窗口(宽、高、窗口名称)
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

    // 初始化GLAD，加载OpenGL函数指针地址的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 指定当前视口尺寸(前两个参数为左下角位置，后两个参数是渲染窗口宽、高)
    glViewport(0, 0, screen_width, screen_height);
	Shader shader("res/shader/task-camera.vs", "res/shader/task-camera.fs");//加载着色器
	GLuint vertex_array_object; // == VAO
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	GLuint vertex_buffer_object; // == VBO
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glEnable(GL_DEPTH_TEST);

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {

		//计算每帧的时间差
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清理颜色缓冲和深度缓冲

		shader.Use();

		// Transform坐标变换矩阵
		glm::mat4 model(1);//model矩阵，局部坐标变换至世界坐标
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 view(1);//view矩阵，世界坐标变换至观察坐标系
		view = camera.GetViewMatrix();
		glm::mat4 projection(1);//projection矩阵，投影矩阵
		projection = glm::perspective(glm::radians(camera.Zoom), screen_width / screen_height, 0.1f, 100.0f);

		// 向着色器中传入参数
		shader.SetMat4("model",model);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);

		//绘制
		glBindVertexArray(vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
    }

	//释放VAOVBO
	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteBuffers(1, &vertex_buffer_object);

	// 清理所有的资源并正确退出程序
    glfwTerminate();
    return 0;
}

//键盘输入
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

//窗口分辨率自适应
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//鼠标移动响应
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
//鼠标滚轮响应
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}




