#include "TestBlending.h"

#include <imgui.h>

namespace test
{
    TestBlending::TestBlending():
        m_CurrentState{
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA,
            GL_FUNC_ADD,
            true
        },
        m_LastState{m_CurrentState},
        m_CurrentSRCFactorIdx{6},       // GL_SRC_ALPHA
        m_CurrentDSTFactorIdx{7},       // GL_ONE_MINUS_SRC_ALPHA
        m_CurrentBlendFuncFuncIdx{0}    // GL_FUNC_ADD
    {
        Update();
    }

    TestBlending::~TestBlending()
    {}

    void TestBlending::OnRender()
    {
        if(m_CurrentState != m_LastState)
        {
            Update();
            m_LastState = m_CurrentState;
        }
    }

    void TestBlending::OnImGuiRender()
    {
        ImGui::Combo("Source factor", &m_CurrentSRCFactorIdx,
            [](void* user_data, int idx) {
                return TestBlending::m_FactorValues[idx].second;
            }, nullptr, m_NumFactors);
        m_CurrentState.sfactor = m_FactorValues[m_CurrentSRCFactorIdx].first;

        ImGui::Combo("Destination factor", &m_CurrentDSTFactorIdx,
            [](void* user_data, int idx) {
                return TestBlending::m_FactorValues[idx].second;
            }, nullptr, m_NumFactors);
        m_CurrentState.dfactor = m_FactorValues[m_CurrentDSTFactorIdx].first;

        ImGui::Combo("Blending function", &m_CurrentBlendFuncFuncIdx,
            [](void* user_data, int idx) {
                return TestBlending::m_FuncValues[idx].second;
            }, nullptr, m_NumFuncs);
        m_CurrentState.blendFunc = m_FuncValues[m_CurrentBlendFuncFuncIdx].first;

        ImGui::Checkbox("Enable blending", &m_CurrentState.enabled);
    }

    void TestBlending::Update()
    {
        GLCall(glBlendFunc(m_CurrentState.sfactor, m_CurrentState.dfactor));
        GLCall(glBlendEquation(m_CurrentState.blendFunc));
        if(m_CurrentState.enabled)
            GLCall(glEnable(GL_BLEND));
        else
            GLCall(glDisable(GL_BLEND));
    }
}
