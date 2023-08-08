#pragma once

#include <vector>

#include "Renderer.h"

struct VertexBufferElement
{
    GLenum type;
    unsigned int count;
    GLenum normalized;

    static unsigned int GetTypeSize(GLenum type)
    {
        switch(type)
        {
        case GL_FLOAT:         return sizeof(GLfloat);
        case GL_UNSIGNED_INT:  return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout():
        m_Stride{0}
    {}

    template<typename T>
    inline void Push(unsigned int count);

    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};

// Template method's definition and specializations are placed here because
// otherwise the code wouldn't comply with the standard
// and sadly won't compile on some platforms
template<typename T>
inline void VertexBufferLayout::Push(unsigned int count)
{
    static_assert(false);
}

template<>
inline void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetTypeSize(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_Stride += count * VertexBufferElement::GetTypeSize(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_Stride += count * VertexBufferElement::GetTypeSize(GL_UNSIGNED_BYTE);
}
