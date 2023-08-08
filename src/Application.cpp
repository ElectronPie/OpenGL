#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

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

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    /* My laptop is a such a potato, this line somehow made the program faster on it KEKW */
    glfwSwapInterval(1);

    /* Load OpenGL with GLAD */
    if(!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Output OpenGL version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
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

        VertexArray va;
        VertexBuffer vb{positions, sizeof(positions)};

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib{indices, (sizeof(indices)/sizeof(indices[0]))};

        auto [vertexSource, fragmentSource] = ParseShader("Basic");
        GLCall(GLuint shader = CreateShader(vertexSource, fragmentSource));
        GLCall(glUseProgram(shader));

        GLCall(GLint location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);

        GLCall(glBindVertexArray(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GLCall(glUseProgram(0));

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while(!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader));
            GLCall(glUniform4f(location,
                r,
                0.63671875f,
                0.40625f,
                1.0f
            ));

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

            if(r >= 1.0f)
                increment = -0.05f;
            else if(r <= 0.0f)
                increment = +0.05f;
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}
