#pragma once

#include "Renderer.h"

#include <string>
#include <unordered_map>

class Shader
{
public:
    Shader(const std::string& respath, const std::string& name);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
    struct ShaderSource
    {
        std::string vertexShader;
        std::string fragmentShader;
    };
    ShaderSource ParseShader(const std::string& respath, const std::string& name);
    unsigned int CompileShader(GLenum type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int GetUniformLocation(const std::string& name);

    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};
