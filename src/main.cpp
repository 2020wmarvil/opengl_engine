// external imports
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
#include "Camera.h"

// constants
#define WIDTH 800
#define HEIGHT 600
#define TARGET_FPS 60

void setup();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;	
float lastFrame = 0.0f; 

float lastMouseX = WIDTH / 2.0f;
float lastMouseY = HEIGHT / 2.0f;

bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

int main() {
	// glfw setup
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwSetWindowTitle(window, "ラストエグザイル");
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback); 

	// opengl setup
	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// generate the data we'll render (hard code for now, eventually read in from an asset file)
	float vertices[] = {
	//	positions				colors					tex coords
		-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// zero
		 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		// one
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		// two
		-0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		// three

		-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		// four
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// five
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		// six
		-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		// seven

		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// eight
		 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		// nine
		 0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		// ten
		 0.5f,  0.5f,  0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		// eleven

		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		// twelve
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// thirteen
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		// fourteen
		-0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		// fifteen

		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		// sixteen
		 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// seventeen
		 0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		// eighteen
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		// nineteen

		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		// twenty
		 0.5f,  0.5f,  0.5f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		// twenty_one
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f, 1.0f,		0.0f, 1.0f,		// twenty_two
		-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		// twenty_three
	};

	unsigned int indices[] {
		0, 1, 2,
		2, 3, 0,
		
		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};

	std::vector<glm::vec3> cubePositions = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};	

	// create renderable objects out of our data
	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices), GL_DYNAMIC_DRAW);

	VertexBufferLayout layout;
	layout.PushFloat(3);
	layout.PushFloat(3);
	layout.PushFloat(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, sizeof(indices), GL_DYNAMIC_DRAW);

	// create the shader and set the uniforms
	Shader shader("../res/shaders/Shader.shader");
	shader.Bind();

	// create and bind a texture
	Texture tex1("../res/textures/rose.jpg");
	Texture tex2("../res/textures/awesomeface.png");

	shader.Bind();
	tex1.Bind(0);
	shader.SetUniform1i("u_Texture1", 0);
	tex2.Bind(1);
	shader.SetUniform1i("u_Texture2", 1);

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

	// game loop
	while(!glfwWindowShouldClose(window)) {
		// manage time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// clear the buffers
		renderer.Clear();

		// update the MVP matrices
		glm::mat4 proj = glm::perspective(glm::radians(camera.GetFOV()), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		for (int i=0; i<cubePositions.size(); i++) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));

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
	    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	    ImGui::End();

		// render the imgui window
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// swap the buffers and poll for events
	 	glfwSwapBuffers(window);
		glfwPollEvents();

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
 
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::LEFT, deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if(firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

	camera.ProcessMouseMovement(xpos - lastMouseX, lastMouseY - ypos);

	lastMouseX = xpos;
	lastMouseY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}