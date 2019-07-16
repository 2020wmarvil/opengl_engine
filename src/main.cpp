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
#define PLAYER_SPEED 0.025

void processInput(GLFWwindow *window, std::vector<float>& speed);
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
		250.0f, 150.0f, 0.0f,  0.0f, 0.0f,
		550.0f, 150.0f, 0.0f,  1.0f, 0.0f,
		550.0f, 450.0f, 0.0f,  1.0f, 1.0f,
		250.0f, 450.0f, 0.0f,  0.0f, 1.0f
	};

	unsigned int indices[] {
		0, 1, 2,
		2, 3, 0
	};

	// create our vertex array object
	VertexArray va;
	VertexBuffer vb(vertices, 4 * 5 * sizeof(float), GL_DYNAMIC_DRAW);

	VertexBufferLayout layout;
	layout.PushFloat(3);
	layout.PushFloat(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 2 * 3 * sizeof(unsigned int));

	glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));

	glm::mat4 mvp = proj * view * model;

	// create the shader and set the uniforms
	Shader shader("../res/shaders/Shader.shader");

	Texture texture("../res/textures/rose.jpg");

	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);
	shader.SetUniformMat4f("u_MVP", mvp);

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

	std::vector<float> speed = { 0.0f, 0.0f };

	// render loop
	while(!glfwWindowShouldClose(window)) {
		// input
		processInput(window, speed);

		mvp = glm::translate(mvp, glm::vec3(speed[0], speed[1], 0.0f));

		// clear the buffers
		renderer.Clear();

		// update the uniforms
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);

		// render stuff!
		renderer.Draw(va, ib, shader);

		// poll for events and swap buffers
		glfwPollEvents();
	 	glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
 
void processInput(GLFWwindow *window, std::vector<float>& speed) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}

	if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		speed[0] = PLAYER_SPEED;
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		speed[0] = -PLAYER_SPEED;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		speed[1] = PLAYER_SPEED;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		speed[1] = -PLAYER_SPEED;
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
		speed[0] = 0.0f;
	}
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
		speed[1] = 0.0f;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
