/***
* 例程  绘制球体 (MAKE后运行时可删除ALL_BUILD，也可以将Task-sphere设为默认启动工程)
* 步骤:
* 1-初始化:       GLFW窗口，GLAD。
* 2-计算球体顶点：通过数学方法计算球体的每个顶点坐标
* 2-数据处理:     通过球体顶点坐标构造三角形网格，生成并绑定VAO&VBO&EBO(准备在GPU中进行处理)，设置顶点属性指针(本质上就是告诉OpenGL如何处理数据)。
* 3-着色器:       给出顶点和片段着色器，然后链接为着色器程序，渲染时使用着色器程序。
* 4-渲染:         使用画线模式画圆，开启面剔除，剔除背面，使用线框模式画球
* 5-结束：        清空缓冲，交换缓冲区检查触发事件后释放资源
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

//将球横纵划分成50X50的网格
const int Y_SEGMENTS = 50;
const int X_SEGMENTS = 50;


int main()
{
	// 初始化GLFW
	glfwInit();                                                     // 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL版本为3.3，主次版本号均设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式(无需向后兼容性)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 如果使用的是Mac OS X系统，需加上这行
	glfwWindowHint(GLFW_RESIZABLE, FALSE);						    // 不可改变窗口大小

																	// 创建窗口(宽、高、窗口名称)
	auto window = glfwCreateWindow(screen_width, screen_height, "Sphere", nullptr, nullptr);
	if (window == nullptr) {                                        // 如果窗口创建失败，输出Failed to Create OpenGL Context
		std::cout << "Failed to Create OpenGL Context" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);                                 // 将窗口的上下文设置为当前线程的主上下文

																	// 初始化GLAD，加载OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 指定当前视口尺寸(前两个参数为左下角位置，后两个参数是渲染窗口宽、高)
	glViewport(0, 0, screen_width, screen_height);




	Shader shader("res/shader/task3.vs", "res/shader/task3.fs");//加载着色器
	
	std::vector<float> sphereVertices;
	std::vector<int> sphereIndices;


	// 生成球的顶点
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

	// 生成球的Indices
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


	// 球
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//生成并绑定球体的VAO和VBO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 将顶点数据绑定至当前默认的缓冲中
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	GLuint element_buffer_object; //EBO
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 解绑VAO和VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 清空颜色缓冲
		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
		//绘制球
		//开启面剔除(只需要展示一个面，否则会有重合)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindVertexArray(VAO);
		//使用线框模式绘制
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, X_SEGMENTS*Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
		//点阵模式绘制
		//glPointSize(5);
		//glDrawElements(GL_POINTS, X_SEGMENTS*Y_SEGMENTS*6, GL_UNSIGNED_INT, 0);
		//交换缓冲并且检查是否有触发事件(比如键盘输入、鼠标移动等）
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// 删除VAO和VBO，EBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &element_buffer_object);

	// 清理所有的资源并正确退出程序
	glfwTerminate();
	return 0;
}


