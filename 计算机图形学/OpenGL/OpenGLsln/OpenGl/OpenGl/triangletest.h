#ifndef TRIANGLETEST_H
#define TRIANGLETEST_H

#include <iostream>
#include <GLFW/glfw3.h>
#include "shader_s.h"
using namespace std;

void helloTriangle(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*));
void helloRectangle(GLFWwindow* window, Shader shaderProgram, void processInput(GLFWwindow*));
void drawTwoTriangles(GLFWwindow* window, Shader shaderProgram, void processInput(GLFWwindow*));
void drawWith2VAO(GLFWwindow* window, Shader shaderProgram, void processInput(GLFWwindow*));
void drawWith2VBO(GLFWwindow* window, Shader shaderProgram, void processInput(GLFWwindow*));

class HelloTriangle {
public:
	void Do(GLFWwindow* window, void processInput(GLFWwindow*)) {
		Shader ourshader("shaders/HelloTriangle/hellotriangle.vertex", "shaders/HelloTriangle/hellotriangle.frag");
		helloTriangle(window, ourshader, processInput);
		//helloRectangle(window, ourshader, processInput);
		//drawTwoTriangles(window, ourshader, processInput);
		//drawWith2VAO(window, ourshader, processInput);

		//Shader ourshader ("shaders/HelloTriangle/doublevertexpointer.vertex", "shaders/HelloTriangle/doublevertexpointer.frag");
		//drawWith2VBO(window, ourshader, processInput);
	}

	// ��ã�VAO,VBO��
	void helloTriangle(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*)) {

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
			shaderProgram.use();
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
	void helloRectangle(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*)) {
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

		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
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
			shaderProgram.use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	// �κ���ҵ��
	//1. ��������������������
	void drawTwoTriangles(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*)) {
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
		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(10);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			shaderProgram.use();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	//2. ������ͬ�����������Σ��������ǵ�����ʹ�ò�ͬ��VAO��VBO
	void drawWith2VAO(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*)) {
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
		unsigned int VBOs[2], VAOs[2];
		glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
		glGenBuffers(2, VBOs);
		// first triangle setup
		// --------------------
		glBindVertexArray(VAOs[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
		glEnableVertexAttribArray(10);
		// glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
		// second triangle setup
		// ---------------------
		glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
		glEnableVertexAttribArray(10);
		// glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)


		// uncomment this call to draw in wireframe polygons.
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// input
			// -----
			processInput(window);

			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.use();
			// draw first triangle using the data from the first VAO
			glBindVertexArray(VAOs[0]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			// then we draw the second triangle using the data from the second VAO
			glBindVertexArray(VAOs[1]);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(2, VAOs);
		glDeleteBuffers(2, VBOs);
	}

	//3.ʹ�ò�ͬVBO����ͬһ������
	void drawWith2VBO(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*)) {
		float triangle[] = {
			0.0f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
		};
		float color[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
		};
		GLuint VBOs[2], VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(2, VBOs);

		//�󶨶���
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(10);

		//����ɫ
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
		glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(11);

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			// input
			// -----
			processInput(window);

			// render
			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			shaderProgram.use();
			// draw first triangle using the data from the first VAO
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		// optional: de-allocate all resources once they've outlived their purpose:
		// ------------------------------------------------------------------------
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(2, VBOs);
	}
};

#endif