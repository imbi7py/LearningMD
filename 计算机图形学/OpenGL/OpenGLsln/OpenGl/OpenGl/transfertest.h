#ifndef  TRANSFER_H
#define TRANSFER_H

#include <iostream>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "stb_image.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace std;
using namespace glm;

void helloTransfer(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*));

class HelloTransfer {
public:
	void Do(GLFWwindow* window, void processInput(GLFWwindow*)) {
		Shader ourshader("shaders/HelloTransfer/transfershader.vertex", "shaders/HelloTransfer/transfershader.frag");
		helloTransfer(window, ourshader, processInput);
	}

	void helloTransfer(GLFWwindow *window, Shader shaderProgram, void processInput(GLFWwindow*)) {
		float vertices[] = {
			// positions          // texture coords
			1.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
			1.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		GLuint VAO, EBO, VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1,&VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);


		GLuint texture1, texture2;
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T ,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char *data = stbi_load("resources/container.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "Failed to load texture" << endl;
		}
		stbi_image_free(data);

		////���õڶ�������
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//���ú����������Ʒ�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//����������˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//��������
		stbi_set_flip_vertically_on_load(true); // ͼƬ��ԭ�������Ͻǣ�OpenGLԭ�������½ǣ�ͨ��stb_image.h �Ѽ��ص�ͼƬ���� y �ᷭת.
		data = stbi_load("resources/bean.jpg", &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "Failed to load texture" << endl;
		}
		stbi_image_free(data);

		shaderProgram.use();
		//����������ͬ����󶨵���ͬ����Ԫ
		shaderProgram.setInt("texture1", 0);
		shaderProgram.setInt("texture2", 1);

		// render loop
		// -----------
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// bind textures on corresponding texture units
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);

			glBindVertexArray(VAO);

			// first container
			mat4 transform = mat4(1.0);    // glm 0.99���ϰ汾��Ҫ�ֶ���ʼ������
			//�������˳��Ϊ��Ҫ�ģ�˳��ͬ��Ч����ͬ
			transform = translate(transform, vec3(0.5f, -0.5f, 0.0f));
			transform = rotate(transform, (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
			transform = scale(transform, glm::vec3(0.5, 0.5, 0.5));
			shaderProgram.setMat4("transform", transform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// second transformation
			//�����͵ı任������ϢȻ������µ�ͼ��
			transform = mat4(1.0);    // glm 0.99���ϰ汾��Ҫ�ֶ���ʼ������
			transform = translate(transform, vec3(-0.5f, 0.5f, 0.0f));
			transform = scale(transform, glm::vec3(0.5, 0.5, 0.5)* sin((float)glfwGetTime()));
			shaderProgram.setMat4("transform", transform);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	//�κ���ҵ
	//����任ע��scale������Ӱ�죬����Ӱ�챾��ߴ磬������Ӱ�������仯

};
#endif