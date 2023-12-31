#include "VertexArray.h"

#include "Renderer.h"

#include "VertexBufferLayout.h"

VertexArray::VertexArray():
    m_IndicesCount{0}
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for(unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(std::uintptr_t)offset));
        offset += element.count * VertexBufferElement::GetTypeSize(element.type);
    }
}

void VertexArray::AddIndexBuffer(const IndexBuffer& ib)
{
    Bind();
    ib.Bind();

    m_IndicesCount = ib.GetCount();
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
