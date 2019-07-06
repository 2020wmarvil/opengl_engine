// external imports
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// standard library imports
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

// constants
#define WIDTH 800
#define HEIGHT 600

// structs
struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

// helper functions
static void genArrays(const float vertices[], const unsigned int indices[], VertexArray* va);

// shader functions
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
static unsigned int compileShader(unsigned int type, const std::string& source);
static ShaderProgramSource parseShader(const std::string& filepath);

// input fuctions
void processInput(GLFWwindow *window, const std::vector<VertexArray*>& vertex_arrays);

// callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
	// initialize glfw
	glfwInit();

	// window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	// sets the refresh rate to that of our monitor
	glfwSwapInterval(1);

	// initialize glew so that we can access opengl functions
	glewInit();

	// create the space in which opengl will render graphics
	glViewport(0, 0, WIDTH, HEIGHT);

	// window properties
	glfwSetWindowTitle(window, "ラストエグザイル");
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);
//	glfwMaximizeWindow(window);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// create the vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	unsigned int indices[] {
		0, 1, 2,
		2, 3, 0
	};

	float vertices2[] = {
		0.0f, 0.75f, 0.0f,
		0.75f, 0.0f, 0.0f,
		0.0f, -0.75f, 0.0f,
		-0.75f, 0.0f, 0.0f
	};

	unsigned int indices2[] {
		3, 2, 1,
		1, 0, 3
	};

	float color_increment = 0.0001f;

	float color[] = {
		0.2f, 0.3f, 0.8f, 1.0f
	};

	VertexArray va1, va2;
	genArrays(vertices, indices, &va1);
	genArrays(vertices2, indices2, &va2);

	std::vector<VertexArray*> vertex_arrays { &va1, &va2 };

	va1.Bind();

	// compile and utilize the shader program
	ShaderProgramSource source = parseShader("res/shaders/Shader.shader");
	unsigned int shader = createShader(source.vertexSource, source.fragmentSource);
	glUseProgram(shader);

	// retrieve the location of the uniforms
	int location = glGetUniformLocation(shader, "u_Color");

	// render loop
	while(!glfwWindowShouldClose(window)) {
		// clear the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render stuff!
		glUniform4f(location, color[0], color[1], color[2], color[3]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (color[0] > 1.0f || color[0] < 0.0f) {
			color_increment *= -1;
		} color[0] += color_increment;

		// input
		processInput(window, vertex_arrays);

		// poll for events and swap buffers
		glfwPollEvents();
	 	glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);

	glfwTerminate();

	return 0;
}

static void genArrays(const float vertices[], const unsigned int indices[], VertexArray* va) {
	VertexBuffer vb(vertices, 4 * 3 * sizeof(float));

	VertexBufferLayout layout;
	layout.PushFloat(3);
	va->AddBuffer(vb, layout);

	IndexBuffer ib(indices, 2 * 3 * sizeof(unsigned int));

	glBindVertexArray(0);
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));

		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);

		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}  

static ShaderProgramSource parseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType { 
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;

	std::string line;
	std::stringstream ss[2];

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			} else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		} else {
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}
 
void processInput(GLFWwindow *window, const std::vector<VertexArray*>& vertex_arrays) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_1) && !glfwGetKey(window, GLFW_KEY_2)) {
		vertex_arrays[0]->Bind();
	}
	if (glfwGetKey(window, GLFW_KEY_2) && !glfwGetKey(window, GLFW_KEY_1)) {
		vertex_arrays[1]->Bind();
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
