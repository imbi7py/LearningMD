#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "triangletest.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int windowInit();
int mngGlPointer();
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HIGHT = 600;

int main()
{
	windowInit();
}

int windowInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);  //����GLFW���ڴ�С
	if (window == NULL)
	{
		std::cout << "GLFW window����ʧ��" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	mngGlPointer();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // ע�ᴰ�ڻص�

	//����
	HelloTriangle triangle;
	triangle.Do(window, processInput);

	//�����ͷ�
	glfwTerminate();

	return 0;
}


// GLAD������������opengl����ָ�룬����gl����֮ǰ��Ҫ��ʼ��glad
int mngGlPointer() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	return 0;
}


// ���ڴ�С���Ļص�
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);  // ����gl��Ⱦ�ӿ�
}

// �����û�����
void processInput(GLFWwindow *window) {
	//esc���˳�
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

