#ifndef COLORTEST_H
#define COLORTEST_H

#include <iostream>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "camera.h"
#include "stb_image.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace std;
using namespace glm;



void helloColor(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*), Camera &camera);

class HelloColor {
private:
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
public :
	void DoWithCamera(GLFWwindow* window, void processInput(GLFWwindow*), Camera &camera) {
		glEnable(GL_DEPTH_TEST);          // ������Ȳ���

		Shader lightingShader("shaders/HelloColor/colorshader.vertex", "shaders/HelloColor/colorshader.frag");
		Shader ambientLightShader("shaders/HelloColor/colorshader.vertex", "shaders/HelloColor/ambientLightingshader.frag");
		Shader lampShader("shaders/HelloColor/lampshader.vertex", "shaders/HelloColor/lampshader.frag");
		helloColor(window, lightingShader, lampShader, processInput, camera);
	}

	void helloColor(GLFWwindow *window, Shader lightingShader, Shader lampShader,void processInput(GLFWwindow*), Camera &camera) {
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f,  0.5f,
			0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			0.5f,  0.5f, -0.5f,
			0.5f,  0.5f,  0.5f,
			0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		GLuint VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

		// λ������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		GLuint lightVAO;
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);
		// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// ���õ�������Ķ������ԣ������ǵĵ���˵����ֻ��λ�����ݣ�
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		
		
		// ѭ����Ⱦ
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//��������
			lightingShader.use();
			lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);  //����������ɫ
			lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);   //���õƹ���ɫ

			mat4 projection = perspective(radians(camera.Zoom), (float)(SCR_WIDTH / SCR_HEIGHT), .1f, 100.0f);
			mat4 view = camera.GetViewMatrix();
			mat4 model = mat4(1.0f);
			lightingShader.setMat4("projection", projection);
			lightingShader.setMat4("view", view);
			lightingShader.setMat4("model", model);
			// ����
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//���ù�Դ
			lampShader.use();
			lampShader.setMat4("projection", projection);
			lampShader.setMat4("view", view);
			model = glm::mat4(1.0f);
			vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f)); //����һ��СС�ķ���
			lampShader.setMat4("model", model);
			// ����
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);



			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteVertexArrays(1, &lightVAO);
		glDeleteBuffers(1, &VBO);

	}
};
#endif