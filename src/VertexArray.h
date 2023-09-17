#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void AddIndexBuffer(const IndexBuffer& ib);

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetIndicesCount() const
    { return m_IndicesCount; }

private:
    unsigned int m_RendererID;
    unsigned int m_IndicesCount;
};
