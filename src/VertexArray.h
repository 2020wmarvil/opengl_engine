#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray {
private:
public: 
    unsigned int m_RendererID;
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};