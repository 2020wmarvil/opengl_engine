// external imports
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// standard library imports
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../src/test.cpp"

// constants
#define WIDTH 800
#define HEIGHT 600

// structs
struct ShaderProgramSource {
	std::string vertexSource;
	std::string fragmentSource;
};

// helper functions
static void genArrays(const float vertices[], const unsigned int indices[], unsigned int *VAO);

// shader functions
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
static unsigned int compileShader(unsigned int type, const std::string& source);
static ShaderProgramSource parseShader(const std::string& filepath);

// input fuctions
void processInput(GLFWwindow *window);

// callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// some useful variables
static unsigned int default_VAO, alternate_VAO;

int main() {
	std::cout << test() << std::endl;
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

	// create the vertex array objects
		// a vertex array object can help us swap between VBOs
	genArrays(vertices, indices, &default_VAO);
	genArrays(vertices2, indices2, &alternate_VAO);

	glBindVertexArray(default_VAO);

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
		processInput(window);

		// poll for events and swap buffers
		glfwPollEvents();
	 	glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);

	glfwTerminate();

	return 0;
}

static void genArrays(const float vertices[], const unsigned int indices[], unsigned int *VAO) {
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	// create the "vertex buffer object"
		// a vertex buffer exists to send batches of vertices to the gpu at once to save time
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	// create the "index buffer object"
		// an index buffer exists so that we can specify the vertex combinations that make up triangles
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_1) && !glfwGetKey(window, GLFW_KEY_2)) {
		glBindVertexArray(default_VAO);
	}
	if (glfwGetKey(window, GLFW_KEY_2) && !glfwGetKey(window, GLFW_KEY_1)) {
		glBindVertexArray(alternate_VAO);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}



/*
// imports
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

// constants
#define WIDTH 800
#define HEIGHT 600
#define SPEED 0.0001f

// input fuctions
void processInput(GLFWwindow *window, std::vector<float> &speed);
void updatePosition(std::vector<std::vector<float>> &points, const std::vector<float> &speed);

// callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
	glfwInit();

	// window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	// create the window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewInit();

	glViewport(0, 0, WIDTH, HEIGHT);

	// window properties
	glfwSetWindowTitle(window, "ラストエグザイル");
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);
//	glfwMaximizeWindow(window);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::vector<std::vector<float>> points {
			{-0.5f, -0.5f},
			{0.0f, 0.5f},
			{0.5f, -0.5f}
	};

	std::vector<float> speed { 0.0f, 0.0f };

	// render loop
	while(!glfwWindowShouldClose(window)) {
		// clear the buffers
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// render stuff!
		glBegin(GL_TRIANGLES);	
			glVertex2f(points[0][0], points[0][1]);
			glVertex2f(points[1][0], points[1][1]);
			glVertex2f(points[2][0], points[2][1]);
		glEnd();

		updatePosition(points, speed);

		// input
		processInput(window, speed);

		// poll for events and swap buffers
		glfwPollEvents();
	 	glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void updatePosition(std::vector<std::vector<float>> &points, const std::vector<float> &speed) {
	points[0][0] += speed[0];
	points[1][0] += speed[0];
	points[2][0] += speed[0];

	points[0][1] += speed[1];
	points[1][1] += speed[1];
	points[2][1] += speed[1];
}

void processInput(GLFWwindow *window, std::vector<float> &speed) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		speed[0] = SPEED;
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		speed[0] = -SPEED;
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		speed[1] = SPEED;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		speed[1] = -SPEED;

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
*/
