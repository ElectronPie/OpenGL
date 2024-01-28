#include "Shader.h"

#include <iostream>
#include <fstream>

#include "Renderer.h"

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

Shader::Shader(const std::string& respath, const std::string& name)
{
    auto [vertexSource, fragmentSource] = ParseShader(respath, name);
    m_RendererID = CreateShader(vertexSource, fragmentSource);
}

Shader::ShaderSource Shader::ParseShader(const std::string& respath, const std::string& name)
{
    std::string fileBaseNameNoExtension = respath;
    if(fileBaseNameNoExtension.empty())
    {
        fileBaseNameNoExtension = "res/shaders/";
    }
    else
    {
        if(fileBaseNameNoExtension.find_last_of("/\\") != (fileBaseNameNoExtension.length() - 1))
        {
            fileBaseNameNoExtension.append("/");
        }
    }
    std::string vertexSource;
    std::string fragmentSource;
    try
    {
        vertexSource = GetFileContents(fileBaseNameNoExtension + name + ".vs");
        fragmentSource = GetFileContents(fileBaseNameNoExtension + name + ".fs");
    }
    catch(std::string s)
    {
        std::cout << s;
        return {"", ""};
    }
    return {vertexSource, fragmentSource};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(GLuint id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, NULL));
    GLCall(glCompileShader(id));

    GLint result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if(result == GL_FALSE)
    {
        GLint length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(GLchar));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

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

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

#define UNIFORM_VECTOR_1(PREFIX, TYPE) \
void Shader::SetUniform1 ## PREFIX (const std::string& name, TYPE value) \
{ \
    GLCall(glUniform1 ## PREFIX (GetUniformLocation(name), value)); \
}

#define UNIFORM_VECTOR_2(PREFIX, TYPE) \
void Shader::SetUniform2 ## PREFIX (const std::string& name, TYPE v0, TYPE v1) \
{ \
    GLCall(glUniform2 ## PREFIX (GetUniformLocation(name), v0, v1)); \
}

#define UNIFORM_VECTOR_3(PREFIX, TYPE) \
void Shader::SetUniform3 ## PREFIX (const std::string& name, TYPE v0, TYPE v1, TYPE v2) \
{ \
    GLCall(glUniform3 ## PREFIX (GetUniformLocation(name), v0, v1, v2)); \
}

#define UNIFORM_VECTOR_4(PREFIX, TYPE) \
void Shader::SetUniform4 ## PREFIX (const std::string& name, TYPE v0, TYPE v1, TYPE v2, TYPE v3) \
{ \
    GLCall(glUniform4 ## PREFIX (GetUniformLocation(name), v0, v1, v2, v3)); \
}

#define UNIFORM_MATRIX_M_X_M(M) \
void Shader::SetUniformMat ## M ## f(const std::string& name, const glm::mat ## M & matrix) \
{ \
    GLCall(glUniformMatrix ## M ## fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0])); \
}

#define UNIFORM_MATRIX_M_X_N(M, N) \
void Shader::SetUniformMat ## M ## x ## N ## f(const std::string& name, const glm::mat ## M ## x ## N & matrix) \
{ \
    GLCall(glUniformMatrix ## M ## x ## N ## fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0])); \
}

UNIFORMS

#undef UNIFORM_VECTOR_1
#undef UNIFORM_VECTOR_2
#undef UNIFORM_VECTOR_3
#undef UNIFORM_VECTOR_4
#undef UNIFORM_MATRIX_M_X_M
#undef UNIFORM_MATRIX_M_X_N

unsigned int Shader::GetUniformLocation(const std::string& name) const
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if(location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}
