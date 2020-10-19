//总体流程（实现是否使用构造切线空间的法线贴图的比较，加载外部模型）
//1. 初始化并创建窗口
//2. 开启深度测试并加载纹理，法线贴图，着色器和模型
//3. 进入主循环清除缓冲
//4. 使用不构造切线空间的着色器，构造并传入pvm矩阵和光线参数，绘制
//5. 使用构造切线空间的着色器，绑定相关顶点数组，构造传入pvm矩阵和光线参数，绘制（切线空间所需的TBN矩阵在顶点着色器中构造生成）
//6. 使用模型，构造传入pvm矩阵和光线参数，并进行绘制
//7. 结束绘制，关闭界面

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "Texture.h"
#include "Camera.h"//相机类
#include "Shader.h"//着色器类

const int screen_width = 1280;
const int screen_height = 720;

GLuint CreateCube();
GLuint CreateCubeWithTangent();

int main() {
    // Init window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto window = glfwCreateWindow(screen_width, screen_height, "tangent", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to Create OpenGL Context" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, screen_width, screen_height);

    GLuint cube_VAO = CreateCube();//加载立方体顶点
    GLuint cube_with_tangent_VAO = CreateCubeWithTangent();
	Texture cube_diffuse;
	unsigned int cube_diffuse_texture=cube_diffuse.LoadTextureFromFile("res/texture/cube_diffuse.jpg");//加载纹理
	Texture cube_normal;
	unsigned int cube_normal_texture= cube_normal.LoadTextureFromFile("res/texture/cube_normal.jpg");//加载法线贴图

    Shader normalmap_shader("res/shader/normal.vs", "res/shader/normal.fs");//加载着色器
    Shader normalmap_shader_in_tangent("res/shader/normal_tangent.vs", "res/shader/normal_tangent.fs");

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

   
    
    while (!glfwWindowShouldClose(window)) {

		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);//清除屏幕清除缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 绘制不在切线空间内的法线贴图
        normalmap_shader.Use();
        glm::mat4 model(1.0f);//计算并向着色器传入pvm矩阵
        model = glm::translate(model, glm::vec3(-1.0f, 0, 0));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, (float)glfwGetTime() * 0.3f, glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)screen_width / screen_height, 0.1f, 100.0f);
        normalmap_shader.SetMat4("model", model);
        normalmap_shader.SetMat4("view", camera.GetViewMatrix());
        normalmap_shader.SetMat4("projection", projection);
        normalmap_shader.SetVec3("view_position", camera.position);//传入参数，视角位置

        normalmap_shader.SetVec3("light_direction", glm::vec3(-0.3f, 0.0f, -1.0f));//光线方向
        normalmap_shader.SetVec3("light_ambient", glm::vec3(0.1f, 0.1f, 0.1f));//环境光参数
        normalmap_shader.SetVec3("light_diffuse", glm::vec3(0.5f, 0.5f, 0.5f));//漫反射参数
        normalmap_shader.SetVec3("light_specular", glm::vec3(1.0f, 1.0f, 1.0f));//镜面反射参数

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube_diffuse_texture);//传入正常纹理和法线贴图 
        normalmap_shader.SetInt("texture_material", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cube_normal_texture);
        normalmap_shader.SetInt("texture_normal", 1);

        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


        // 绘制在切线空间内的法线贴图
        normalmap_shader_in_tangent.Use();
        model = glm::mat4(1.0f);//pvm矩阵
        model = glm::translate(model, glm::vec3(1.0f, 0, 0));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, (float)glfwGetTime() * 0.3f, glm::vec3(0.5f, 1.0f, 0.0f));
		normalmap_shader_in_tangent.SetMat4("model", model);
        normalmap_shader_in_tangent.SetMat4("view", camera.GetViewMatrix());
        normalmap_shader_in_tangent.SetMat4("projection",projection);
        normalmap_shader_in_tangent.SetVec3("view_position", camera.position);
		//光线参数
        normalmap_shader_in_tangent.SetVec3("light_direction", glm::vec3(-0.3f, 0.0f, -1.0f));
        normalmap_shader_in_tangent.SetVec3("light_ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        normalmap_shader_in_tangent.SetVec3("light_diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        normalmap_shader_in_tangent.SetVec3("light_specular", glm::vec3(1.0f, 1.0f, 1.0f));
		//传入正常纹理和法线贴图 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cube_diffuse_texture);
        normalmap_shader_in_tangent.SetInt("texture_material", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cube_normal_texture);
        normalmap_shader_in_tangent.SetInt("texture_normal", 1);

        glBindVertexArray(cube_with_tangent_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

		
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLuint CreateCube() {
	//绑定不实现切线空间和法线贴图的VAOVBO
    float vertices[] = {
    // 顶点             法线              纹理坐标
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,          
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
   
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 
    
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 
  
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,         
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,  
   
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 
   
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 
     1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,   
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f   
};

    GLuint vertex_array_object; // == VAO
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint vertex_buffer_object; // == VBO
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

     
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertex_array_object;
}

GLuint CreateCubeWithTangent() {
	//绑定实现切线空间和法线贴图的VAOVBO
	float vertices_with_tangent[] = {
		//顶点                法线                纹理坐标       T
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	};
    GLuint vertex_array_object; // == VAO
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    GLuint vertex_buffer_object; // == VBO
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_with_tangent), vertices_with_tangent, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vertex_array_object;
}