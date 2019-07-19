#pragma once

class IndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Size;
public:
    IndexBuffer(const unsigned int* data, unsigned int size, unsigned int draw_type);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetSize() const { return m_Size; }
};