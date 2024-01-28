#include "TestTexture2D.h"

#include "Renderer.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>

namespace test
{
    TestTexture2D::TestTexture2D():
        m_TranslationA{200.0f, 200.0f, 0.0f},
        m_TranslationB{400.0f, 400.0f, 0.0f},
        m_Proj{glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)},
        m_View{glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))}
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
            +50.0f, -50.0f, 1.0f, 0.0f, // 1
            +50.0f, +50.0f, 1.0f, 1.0f, // 2
            -50.0f, +50.0f, 0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_VAO = std::make_unique<VertexArray>();

        VertexBuffer vb{positions, sizeof(positions)};
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(vb, layout);

        IndexBuffer ib{indices, (sizeof(indices)/sizeof(indices[0]))};
        m_VAO->AddIndexBuffer(ib);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        m_Shader = std::make_unique<Shader>("", "Basic");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("", "Pie");
        m_Texture->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);

        glm::vec4 clear_color(0.0f, 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform4f("u_Color", clear_color.r, clear_color.g, clear_color.b, clear_color.a);

        m_VAO->Unbind();
        vb.Unbind();
        ib.Unbind();
        m_Shader->Unbind();

        m_TestBlending = std::make_unique<TestBlending>();
    }

    TestTexture2D::~TestTexture2D()
    {}

    void TestTexture2D::OnRender()
    {
        Renderer renderer;

        renderer.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.Clear();

        m_TestBlending->OnRender();

        m_Shader->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_Shader);
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("Model position A", (float*)&m_TranslationA.x, 0.0f, 600.0f);
        ImGui::SliderFloat3("Model position B", (float*)&m_TranslationB.x, 0.0f, 600.0f);

        ImGui::End();
        ImGui::Begin("Blending");

        m_TestBlending->OnImGuiRender();
    }
}
