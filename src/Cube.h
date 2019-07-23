#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class Cube {
public:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;

    glm::vec3 m_Color;

    Cube();
    ~Cube();

    void Bind() const;
    void Unbind() const;

    inline glm::vec3 GetColor() const { return m_Color; }
    //inline void SetColor(glm::vec3 color) const { m_Color = color; }
};