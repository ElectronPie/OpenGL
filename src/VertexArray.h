#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void AddIndexBuffer(const IndexBuffer& ib);

    void Bind() const;
    void Unbind() const;
};
