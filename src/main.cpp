// external imports
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// standard library imports
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>

// our library imports
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// constants
#define WIDTH 800
#define HEIGHT 600
#define PLAYER_SPEED 10
#define TARGET_FPS 60

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
		-50.0f, -50.0f, 0.0f,  0.0f, 0.0f,
		 50.0f, -50.0f, 0.0f,  1.0f, 0.0f,
		 50.0f,  50.0f, 0.0f,  1.0f, 1.0f,
		-50.0f,  50.0f, 0.0f,  0.0f, 1.0f
	};

	unsigned int indices[] {
		0, 1, 2,
		2, 3, 0
	};

	std::vector<float> speed = { 0.0f, 0.0f };

	// create our vertex array object
	VertexArray va;
	VertexBuffer vb(vertices, 4 * 5 * sizeof(float), GL_DYNAMIC_DRAW);

	VertexBufferLayout layout;
	layout.PushFloat(3);
	layout.PushFloat(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 2 * 3 * sizeof(unsigned int));

	// create the MVP matrices
	glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// create the shader and set the uniforms
	Shader shader("../res/shaders/Shader.shader");
	shader.Bind();

	// create and bind a texture
	Texture texture("../res/textures/rose.jpg");

	shader.Bind();
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	// unbind our objects
	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	Renderer renderer;

	// create the imgui context
	ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::StyleColorsDark();

	double lasttime = glfwGetTime();
	
	std::vector<glm::vec3> translations = {
		glm::vec3(150.0f, 300.0f, 0.0f),
		glm::vec3(350.0f, 300.0f, 0.0f)
	};

	// game loop
	while(!glfwWindowShouldClose(window)) {
		// poll for events and input
		glfwPollEvents();
		processInput(window, speed);

		// update the world state
		translations[0][0] += speed[0];
		translations[0][1] += speed[1];

		// clear the buffers
		renderer.Clear();

		// update the MVP matrices for each object we are rendering
		for (int i=0; i<translations.size(); i++) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translations[i]);
			glm::mat4 mvp = proj * view * model;

			// set the uniforms
			shader.Bind();
			shader.SetUniformMat4f("u_MVP", mvp);

			// render!
			renderer.Draw(va, ib, shader);
		}

        // start the imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		// create an imgui window
	    ImGui::Begin("debug");
		for (int i=0; i<translations.size(); i++) {
	    	ImGui::SliderFloat3(("Translation" + std::to_string(i)).c_str(), glm::value_ptr(translations[i]), 0.0f, (float)WIDTH);
		}
	    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	    ImGui::End();

		// render the imgui window
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap the buffers
	 	glfwSwapBuffers(window);

		// cap the framerate
	    while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
	    } lasttime += 1.0/TARGET_FPS;
	}

	// terminate imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	// terminate glfw
    glfwDestroyWindow(window);
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