#include "Cube.h"

float vertices[] = {
//	positions				normals					colors				tex coords
	-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// zero
	 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// one
	 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// two
	-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// three

	-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// four
	 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// five
	 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// six
	-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// seven

	 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// eight
	 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// nine
	 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// ten
	 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// eleven

	-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// twelve
	-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// thirteen
	-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// fourteen
	-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// fifteen

	-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// sixteen
	 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// seventeen
	 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// eighteen
	-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// nineteen

	-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// twenty
	 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// twenty_one
	 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// twenty_two
	-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// twenty_three
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

Cube::Cube() 
    : m_VBO(vertices, sizeof(vertices), GL_DYNAMIC_DRAW), m_IBO(indices, sizeof(indices), GL_DYNAMIC_DRAW) 
{
	VertexBufferLayout layout;
	layout.PushFloat(3);
	layout.PushFloat(3);
	layout.PushFloat(3);
	layout.PushFloat(2);
	m_VAO.AddBuffer(m_VBO, layout);

	Cube::m_Color = glm::vec3(1.0f, 0.5f, 0.31f);
}

Cube::~Cube() {
    m_VAO.~VertexArray();
    m_VBO.~VertexBuffer();
    m_IBO.~IndexBuffer();
}

void Cube::Bind() const {
	m_VAO.Bind();
	m_VBO.Bind();
	m_IBO.Bind();
}

void Cube::Unbind() const {
	m_VAO.Unbind();
	m_VBO.Unbind();
	m_IBO.Unbind();
}