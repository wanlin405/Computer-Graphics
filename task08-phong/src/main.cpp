/***
 * Task6  多光源 (MAKE后运行时删除ALL_BUILD，也可以将Task-phong设为默认启动工程)
 * 步骤:
 * 1-初始化:   GLFW窗口，GLAD。
 * 2-数据处理: 给定顶点数据，生成并绑定VAO&VBO(准备在GPU中进行处理)，设置顶点属性指针(本质上就是告诉OpenGL如何处理数据)。
 * 3-光源:     设置了定向光，点光源，聚光的位置和颜色。
 * 4-着色器:   给出立方体和灯的着色器，然后在Shader类中分别链接为着色器程序，渲染时使用着色器程序。
 * 5-摄像机:   计算出每帧的间隔时间，以及摄像机的摆放位置。
 * 6-渲染:     计算间隔时间，输入控制，清空缓冲，开启光源，绑定纹理，绘制立方体和灯，交换缓冲区检查触发事件后释放资源。
 * 注：        键盘控制1~8可控制开关灯，2~7是不同颜色的点光源，1是平行光，8是手电筒聚光
 */

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "iostream"
#include "sstream"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

// 屏幕宽，高
float SCREEN_WIDTH = 1280.0f;
float SCREEN_HRIGHT = 720.0f;

float delta_time = 0.0f;  // 当前帧与上一帧的时间差
float last_frame = 0.0f;  // 上一帧的时间

float last_x = SCREEN_WIDTH / 2.0f, last_y = SCREEN_HRIGHT / 2.0f;   // 设置鼠标初始位置为屏幕中心
unsigned int light_bits = 0;                                         // 控制开关灯

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));                 // 摄像机位置

// 顶点数据																 
float vertices[] = {
	// ---- 位置 ----       ---- 法线 ----   
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// 点光源位置
glm::vec3 point_light_positions[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, 2.0f),
	glm::vec3(0.0f, -2.5f, 0.0f),
	glm::vec3(0.0f, 0.0f, -3.0f),
	glm::vec3(-3.5f, 0.0f, 0.0f)
};

// 光源颜色
glm::vec3 light_colors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.5f, 0.5f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.5f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.5f, 0.0f, 0.5f)
};

void ProcessInput(GLFWwindow *window);
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos);
void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
bool IsLightOn(int index);

int main()
{
    // ---------------------初始化--------------------------
    // glfw初始化，采用的GL版本为3.3核心版本
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // 创建GL窗口
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HRIGHT, "Phong", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 指定上下文为当前窗口
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);            // 隐藏光标，鼠标停留在窗口内
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glfwSetKeyCallback(window, KeyCallback);

    // 初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed!" << std::endl;
        return -1;
    }

    // 开启深度测试并翻转y坐标，由于y坐标正常从底部到顶部是依次增大的，如果不翻转，计算偏移量时要反过来
    glEnable(GL_DEPTH_TEST);
    camera.flip_y = true;

	// 着色器(立方体和灯)
    Shader lamp_shader = Shader("res/shader/DrawLamp.vs", "res/shader/DrawLamp.fs");
    Shader cube_shader = Shader("res/shader/DrawCube.vs", "res/shader/DrawCube.fs");

	// 设置1个定向光，6个点光源
    DirectLight dirLight = DirectLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
    PointLight pointLights[6];

    // 点光源
    for (int i = 0; i < 6; i++)
    {
        pointLights[i] = PointLight(point_light_positions[i], 0.05f * light_colors[i], 0.8f * light_colors[i], light_colors[i], 1.0f, 0.09f, 0.032f);
    }

	// 聚光
    SpotLight spotLight = SpotLight(camera.position, camera.forward, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f),
        1.0f, 0.09f, 0.032f, cos(glm::radians(12.5f)), cos(glm::radians(15.0f)));

    // ---------------------绑定顶点数组对象----------------------
    unsigned int vbo, lamp_vao, cube_vao;
    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cube_vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &lamp_vao);
    glBindVertexArray(lamp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    last_frame = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
		// 在每一帧中计算出新的deltaTime
        float current_frame = glfwGetTime();                 
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

		// 输入控制
        ProcessInput(window);                                

		// 清除颜色和深度缓冲
        glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), SCREEN_WIDTH / SCREEN_HRIGHT, 0.1f, 100.0f);

		// 给立方体一个青色塑料(Cyan Plastic)的材质
		cube_shader.Use();
		cube_shader.SetVec3("viewPos", camera.position);
		cube_shader.SetFloat("material.shininess", 32.0f);
		cube_shader.SetVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		cube_shader.SetVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);

		// 平行光光源
		dirLight.SetOn(IsLightOn(0));
		dirLight.Draw(cube_shader, "dirLight");

		// 点光源
		for (int i = 0; i < 6; i++)
		{
			std::stringstream name_stream;
			name_stream << "pointLights[" << i << "]";
			std::string name = name_stream.str();
			pointLights[i].SetOn(IsLightOn(i + 1));
			pointLights[i].Draw(cube_shader, name);
		}

		// 手电筒光源
		spotLight.SetOn(IsLightOn(7));
		spotLight.SetPosition(camera.position);
		spotLight.SetDirection(camera.forward);
		spotLight.Draw(cube_shader, "spotLight");

		cube_shader.SetMat4("projection", projection);
		cube_shader.SetMat4("view", view);

		// 绘制立方体
		glBindVertexArray(cube_vao);
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, current_frame, glm::vec3(1.0f, 0.3f, 0.5f));
		cube_shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        lamp_shader.Use();
        lamp_shader.SetMat4("projection", projection);
        lamp_shader.SetMat4("view", view);

		// 绘制灯
        glBindVertexArray(lamp_vao);
        for (int i = 0; i < 6; i++)
        {
            if (IsLightOn(i + 1))
            {
                glm::mat4 model(1.0f);
                model = glm::translate(model, point_light_positions[i]);
                model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                lamp_shader.SetMat4("model", model);
                lamp_shader.SetVec3("lightColor", light_colors[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lamp_vao);
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
    return 0;
}

// 键盘控制(摄像机位置WSAD -> 上下左右)
void ProcessInput(GLFWwindow *window)
{
	float speed = camera.mouse_speed * delta_time;
	// 移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Move(camera.forward * speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Move(-camera.forward * speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Move(-camera.right * speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Move(camera.right * speed);
}

// 窗口大小调整的回调函数(当窗口大小改变时，视口也要改变)
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// 键盘控制回调(ESC -> 退出，1~8 -> 控制开关灯)
void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); break;
		case GLFW_KEY_1: light_bits ^= 1; break;
		case GLFW_KEY_2: light_bits ^= (1 << 1); break;
		case GLFW_KEY_3: light_bits ^= (1 << 2); break;
		case GLFW_KEY_4: light_bits ^= (1 << 3); break;
		case GLFW_KEY_5: light_bits ^= (1 << 4); break;
		case GLFW_KEY_6: light_bits ^= (1 << 5); break;
		case GLFW_KEY_7: light_bits ^= (1 << 6); break;
		case GLFW_KEY_8: light_bits ^= (1 << 7); break;
		default:
			break;
		}
	}
}

// 鼠标控制回调
void MouseCallback(GLFWwindow* window, double x_pos, double y_pos)
{
	float x_offset = x_pos - last_x;       // 计算当前帧与上一帧的鼠标位置偏移量
	float y_offset = y_pos - last_y;
	last_x = x_pos;
	last_y = y_pos;

	float sensitivity = 0.1;               // 灵敏度*偏移量，避免鼠标移动太大
	x_offset *= sensitivity;
	y_offset *= sensitivity;

	camera.Rotate(glm::vec3(x_offset, y_offset, 0));
}

// 滚轮控制回调
void ScrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
	camera.Zoom(y_offset);
}

// 控制开关灯
bool IsLightOn(int index)
{
	return (light_bits >> index) & 1;
}


