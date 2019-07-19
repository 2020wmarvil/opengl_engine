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
#define PLAYER_SPEED 0.25
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

	glfwSwapInterval(1);

	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetWindowTitle(window, "ラストエグザイル");
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	// create our vertex array object
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

	glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, -3.0f);
	float camera_angle = 0.0f;

	std::vector<float> speed = { 0.0f, 0.0f };

	// create the MVP matrices
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);

	double lasttime = glfwGetTime();

	// game loop
	while(!glfwWindowShouldClose(window)) {
		// input
		processInput(window, speed);

		// update the world state
		cubePositions[0][0] += speed[0];
		cubePositions[0][1] += speed[1];

		// clear the buffers
		renderer.Clear();

		// update the MVP matrices for each object we are rendering
		glm::mat4 view = glm::translate(glm::mat4(1.0f), camera_pos);
		view = glm::rotate(view, glm::radians(camera_angle), glm::vec3(1.0f, 0.0f, 0.0f));

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
		ImGui::SliderFloat("CameraAngle", &camera_angle, -180.0f, 180.0f);
	    ImGui::SliderFloat3("camera_pos", glm::value_ptr(camera_pos), -250.0f, 250.0f);
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