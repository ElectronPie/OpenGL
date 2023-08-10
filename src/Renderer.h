#pragma once

#include <glad/glad.h>

#include <debugbreak.h>

#include "VertexArray.h"
#include "Shader.h"

#ifdef DEBUG
    #define ASSERT(x) if(!(x)) debug_break()
#else
    #define ASSERT(x)
#endif

#ifdef DEBUG
    #define GLCall(x) \
        GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    void Draw(const VertexArray& va, const Shader& shader);
    void Clear();
};
