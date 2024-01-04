#pragma once

#include "Test.h"

#include <memory>

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "TestBlending.h"

#include <glm/glm.hpp>

namespace test
{
    class TestTexture2D: public Test
    {
    public:
        TestTexture2D();
        ~TestTexture2D();

        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::vec3 m_TranslationA;
        glm::vec3 m_TranslationB;
        glm::mat4 m_Proj;
        glm::mat4 m_View;

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        std::unique_ptr<TestBlending> m_TestBlending;
    };
}
