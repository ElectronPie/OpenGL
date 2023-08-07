#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "debugbreak.h"

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

static void GLClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
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

struct ShaderSource
{
    std::string vertexShader;
    std::string fragmentShader;
};

static std::string GetFileContents(const std::string& filepath)
{
    std::ifstream file{filepath};
    if(!file)
    {
        throw (std::string{"Can't open file "} + filepath + '\n');
    }

    std::string res;
    for(std::string line; std::getline(file, line); res += (line + '\n'));

    file.close();

    return res;
}

static ShaderSource ParseShader(const std::string& name)
{
    std::string vertexShaderFilepath;
    std::string fragmentShaderFilepath;
    vertexShaderFilepath.append("res/shaders/").append(name).append(".vs");
    fragmentShaderFilepath.append("res/shaders/").append(name).append(".fs");
    std::string vertexSource;
    std::string fragmentSource;
    try
    {
        vertexSource = GetFileContents(vertexShaderFilepath);
        fragmentSource = GetFileContents(fragmentShaderFilepath);
    }
    catch(std::string s)
    {
        std::cout << s;
        return {"", ""};
    }
    return {vertexSource, fragmentSource};
}

static GLuint CompileShader(GLenum type, const std::string& source)
{
    GLCall(GLuint id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, NULL));
    GLCall(glCompileShader(id));

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(GLchar));
        glGetShaderInfoLog(id, length, &length, message);

        const char* typeName;
        switch(type)
        {
        case GL_VERTEX_SHADER:
            typeName = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            typeName = "fragment";
            break;
        }
        std::cout << "Failed to compile " << typeName << " shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(GLuint program = glCreateProgram());
    GLCall(GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
    {
        std::cout << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if(!window)
    {
        std::cout << "Failed to create a GLFW Window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Load OpenGL with GLAD */
    if(!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Output OpenGL version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
    GLCall(glEnableVertexAttribArray(0));

    GLuint ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    auto [vertexSource, fragmentSource] = ParseShader("Basic");
    GLCall(GLuint shader = CreateShader(vertexSource, fragmentSource));
    GLCall(glUseProgram(shader));

    GLCall(GLint location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location,
        0.265625f,
        0.63671875f,
        0.40625f,
        1.0f
    ));

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}