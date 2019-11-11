#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int windowInit();
int mngGlPointer();
int createShaderProgram();
void helloTriangle(GLFWwindow *window, int shaderProgram);
void helloRectangle(GLFWwindow *window, int shaderProgram);
void drawTwoTriangles(GLFWwindow *window, int shaderProgram);
void drawWith2VAO(GLFWwindow *window, int shaderProgram);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 10) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.3f, 0.5f, 1.0f);\n"
"}\n\0";


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

	int shaderProgram = createShaderProgram();

	//helloTriangle(window,shaderProgram);
	//helloRectangle(window, shaderProgram);
	//drawTwoTriangles(window, shaderProgram);
	drawWith2VAO(window, shaderProgram);
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

int createShaderProgram() {
	/*******************����������shader����    START************************/
	//����������ɫ��
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//����Ƿ�������
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "������ɫ������ʧ�ܣ�\n" << infoLog << std::endl;
	}

	//����Ƭ����ɫ��
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//����Ƿ�������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Ƭ����ɫ������ʧ�ܣ�\n" << infoLog << std::endl;
	}

	//������ɫ��
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//����Ƿ�������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "��ɫ������ʧ�ܣ�" << infoLog << std::endl;
	}
	//������Ӻ�ɾ��ԭshader����
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	/*******************����������shader����     END*******************/
	return shaderProgram;
}

// ��ã�VAO,VBO��
void helloTriangle(GLFWwindow *window, int shaderProgram) {

	/*******************���ö�������     START*******************/
	//��ʼ��������
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	//���Ȱ󶨶����������Ȼ��󶨲����ö��㻺������Ȼ�����ö������ԡ�
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	//��VAO
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ���ö�������ָ��
	glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(10);
	//��ע�⣬��������ģ�
	//�� glVertexAttribPointer �ĵ��ý� VBO ע��Ϊ�������Եİ󶨶��㻺��������
	//���֮�����ǿ��԰�ȫ��ȡ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//֮�����ȡ���� VAO���Ա����� VAO ���ò��������޸Ĵ� VAO��������������ٷ�����
	//�޸�����VAO ��Ҫ���� glBindVertexArray����ˣ��ڷ�ֱ����Ҫʱ������ͨ������ȡ���� VAO��
	glBindVertexArray(0);
	/*******************���ö�������     END*******************/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //�����߿���Ⱦģʽ

	// ѭ����Ⱦ
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //ʹ�ø�����ɫ������ɫ������
		glClear(GL_COLOR_BUFFER_BIT);

		//����������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);  //��Ȼֻ��ֻһ��VAO����û��Ҫÿ��ѭ������һ�£�����Ϊ�˹淶

		//glDrawArrays(GL_LINES, 0, 2);    // ������
		//glDrawArrays(GL_LINE_LOOP, 0, 3);  //������β��������
		glDrawArrays(GL_TRIANGLES, 0, 3);

 		// glBindVertexArray(0);     //û��Ҫÿ�ζ����

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

//��ã�EBO��
void helloRectangle(GLFWwindow *window, int shaderProgram) {
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	GLuint indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *) 0);
	glEnableVertexAttribArray(10);

	//���û�����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

// �κ���ҵ��
//1. ��������������������
void drawTwoTriangles(GLFWwindow *window, int shaderProgram) {
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//�ȿӵ㣺�ڶ��������ĺ����Ƕ����ά��
	glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void* )0);
	glEnableVertexAttribArray(10);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  
		glClear(GL_COLOR_BUFFER_BIT);

		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);  
		glDrawArrays(GL_TRIANGLES, 0,6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

//2. ������ͬ�����������Σ��������ǵ�����ʹ�ò�ͬ��VAO��VBO
void drawWith2VAO(GLFWwindow *window, int shaderProgram) {
	float firstTriangle[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};
	float secondTriangle[] = {
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
	};

	//VAO��VBO�����������飬����������ô�鷳
	//GLuint VAO1, VBO1,VAO2,VBO2;

	GLuint VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glBindVertexArray(VAOs[0]);
	glGenBuffers(2, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	// glBindVertexArray(0); // ��Ϊ������Ҫ��VAO������û��Ҫ���
	// second triangle setup
	// ---------------------
	glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
	glEnableVertexAttribArray(0);
	// glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)

	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shaderProgram);
		/*glBindVertexArray(tar);
		if (tar == VAO1) {
			tar = VAO2;
		}
		else
		{
			tar = VAO1;
		}*/
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// then we draw the second triangle using the data from the second VAO
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);*/
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
}