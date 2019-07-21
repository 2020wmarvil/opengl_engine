#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class Cube {
private:
public:
    VertexArray m_VAO;      // i want these to be private
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;

    Cube();
    ~Cube();

    void Bind() const;
    void Unbind() const;

    inline VertexArray GetVAO() const { return m_VAO; }         // i want these to work
    inline IndexBuffer GetIBO() const { return m_IBO; }
};