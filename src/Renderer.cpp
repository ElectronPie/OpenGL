#include "Renderer.h"

#include <iostream>
#include <iomanip>

void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] ("
            << std::showbase << std::hex << error
            << std::resetiosflags(std::ios_base::showbase | std::ios_base::hex)
            << "): " << function << " "
            << file << ":" << line << std::endl;
        return false;
    }
    return true;
}