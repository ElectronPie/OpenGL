#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#define UNIFORMS_VECTOR \
    UNIFORM_VECTOR_1(i, int) \
    UNIFORM_VECTOR_1(ui, unsigned int) \
    UNIFORM_VECTOR_1(f, float) \
    UNIFORM_VECTOR_2(i, int) \
    UNIFORM_VECTOR_2(ui, unsigned int) \
    UNIFORM_VECTOR_2(f, float) \
    UNIFORM_VECTOR_3(i, int) \
    UNIFORM_VECTOR_3(ui, unsigned int) \
    UNIFORM_VECTOR_3(f, float) \
    UNIFORM_VECTOR_4(i, int) \
    UNIFORM_VECTOR_4(ui, unsigned int) \
    UNIFORM_VECTOR_4(f, float)

#define UNIFORMS_MATRIX \
    UNIFORM_MATRIX_M_X_M(2) \
    UNIFORM_MATRIX_M_X_M(3) \
    UNIFORM_MATRIX_M_X_M(4) \
    UNIFORM_MATRIX_M_X_N(2, 3) \
    UNIFORM_MATRIX_M_X_N(2, 4) \
    UNIFORM_MATRIX_M_X_N(3, 2) \
    UNIFORM_MATRIX_M_X_N(3, 4) \
    UNIFORM_MATRIX_M_X_N(4, 2) \
    UNIFORM_MATRIX_M_X_N(4, 3)

#define UNIFORMS \
    UNIFORMS_VECTOR \
    UNIFORMS_MATRIX

class Shader
{
public:
    Shader(const std::string& respath, const std::string& name);
    ~Shader();

    void Bind() const;
    void Unbind() const;

#define UNIFORM_VECTOR_1(PREFIX, TYPE) \
    void SetUniform1 ## PREFIX (const std::string& name, TYPE value);

#define UNIFORM_VECTOR_2(PREFIX, TYPE) \
    void SetUniform2 ## PREFIX (const std::string& name, TYPE v0, TYPE v1);

#define UNIFORM_VECTOR_3(PREFIX, TYPE) \
    void SetUniform3 ## PREFIX (const std::string& name, TYPE v0, TYPE v1, TYPE v2);

#define UNIFORM_VECTOR_4(PREFIX, TYPE) \
    void SetUniform4 ## PREFIX (const std::string& name, TYPE v0, TYPE v1, TYPE v2, TYPE v3);

#define UNIFORM_MATRIX_M_X_M(M) \
    void SetUniformMat ## M ## f(const std::string& name, const glm::mat ## M & matrix);

#define UNIFORM_MATRIX_M_X_N(M, N) \
    void SetUniformMat ## M ## x ## N ## f(const std::string& name, const glm::mat ## M ## x ## N & matrix);

    UNIFORMS

#undef UNIFORM_VECTOR_1
#undef UNIFORM_VECTOR_2
#undef UNIFORM_VECTOR_3
#undef UNIFORM_VECTOR_4
#undef UNIFORM_MATRIX_M_X_M
#undef UNIFORM_MATRIX_M_X_N

private:
    struct ShaderSource
    {
        std::string vertexShader;
        std::string fragmentShader;
    };
    ShaderSource ParseShader(const std::string& respath, const std::string& name);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int GetUniformLocation(const std::string& name) const;

    unsigned int m_RendererID;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};
