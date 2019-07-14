// external imports
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// standard library imports
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// our library imports
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// constants
#define WIDTH 800
#define HEIGHT 600

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	// sets the refresh rate to that of our monitor
	glfwSwapInterval(1);

	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetWindowTitle(window, "ラストエグザイル");
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f
	};

	unsigned int indices[] {
		0, 1, 2,
		2, 3, 0
	};

	float color_increment = 0.0001f;

	float color[] = {
		0.2f, 0.3f, 0.8f, 1.0f
	};

	// create our vertex array object
	VertexArray va;
	VertexBuffer vb(vertices, 4 * 5 * sizeof(float));

	VertexBufferLayout layout;
	layout.PushFloat(3);
	layout.PushFloat(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 2 * 3 * sizeof(unsigned int));

	glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

	// create the shader and set the uniforms
	Shader shader("../res/shaders/Shader.shader");
//	shader.Bind();
//	shader.SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);

//	Texture texture("res/textures/cherno_image.png");
	Texture texture("../res/textures/rose.jpg");

	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);
	shader.SetUniformMat4f("u_MVP", proj);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

	// render loop
	while(!glfwWindowShouldClose(window)) {
		// clear the buffers
		renderer.Clear();

		// update the uniforms
//		shader.Bind();
//		shader.SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);
		shader.SetUniformMat4f("u_MVP", proj);

		// render stuff!
		renderer.Draw(va, ib, shader);

		if (color[0] > 1.0f || color[0] < 0.0f) {
			color_increment *= -1;
		} color[0] += color_increment;

		// input
		processInput(window);

		// poll for events and swap buffers
		glfwPollEvents();
	 	glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
 
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
