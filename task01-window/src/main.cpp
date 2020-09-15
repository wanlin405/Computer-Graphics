/***
 * 例程  绘制窗口 (MAKE后运行时可删除ALL_BUILD，也可以将Task-window设为默认启动工程)
 * 步骤:
 * 1-初始化:   GLFW窗口，GLAD。
 * 2-渲染:     清空缓冲，交换缓冲区检查触发事件后释放资源        
 */

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// 屏幕宽，高
int screen_width = 1280;
int screen_height = 720;

int main() {
    // 初始化GLFW
    glfwInit();                                                     // 初始化GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // OpenGL版本为3.3，主次版本号均设为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 使用核心模式(无需向后兼容性)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 如果使用的是Mac OS X系统，需加上这行
    glfwWindowHint(GLFW_RESIZABLE, false);						    // 不可改变窗口大小

    // 创建窗口(宽、高、窗口名称)
    auto window = glfwCreateWindow(screen_width, screen_height, "window", nullptr, nullptr);
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

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {

		// 清空颜色缓冲
		glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// 交换缓冲并且检查是否有触发事件(比如键盘输入、鼠标移动等）
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	// 清理所有的资源并正确退出程序
    glfwTerminate();
    return 0;
}