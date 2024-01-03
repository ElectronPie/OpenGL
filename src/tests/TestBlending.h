#pragma once

#include "Test.h"

#include "Renderer.h"

#define TEST_BLENDING_PAIR_VALUE_NAME(x) {x, #x}

namespace test
{
    class TestBlending : public Test
    {
    public:
        TestBlending();
        ~TestBlending();

        void OnRender() override;
        void OnImGuiRender() override;

    private:
        struct m_State_t
        {
            GLenum sfactor;
            GLenum dfactor;
            GLenum blendFunc;
            bool enabled;

            inline bool operator!=(m_State_t& other)
            {
                return sfactor != other.sfactor ||
                       dfactor != other.dfactor ||
                       blendFunc != other.blendFunc ||
                       enabled != other.enabled;
            }
        } m_CurrentState, m_LastState;

        constexpr static inline std::pair<GLenum, const char*> m_FactorValues[] = {
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ZERO),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ONE),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_SRC_COLOR),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ONE_MINUS_SRC_COLOR),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_DST_COLOR),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ONE_MINUS_DST_COLOR),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_SRC_ALPHA),              // Default Source factor (index 6)
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ONE_MINUS_SRC_ALPHA),    // Default Destination factor (index 7)
            TEST_BLENDING_PAIR_VALUE_NAME(GL_DST_ALPHA),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ONE_MINUS_DST_ALPHA),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_CONSTANT_COLOR),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ONE_MINUS_CONSTANT_COLOR),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_CONSTANT_ALPHA),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_ONE_MINUS_CONSTANT_ALPHA)
        };
        constexpr static inline std::size_t m_NumFactors = sizeof(m_FactorValues) / sizeof(*m_FactorValues);

        constexpr static inline std::pair<GLenum, const char*> m_FuncValues[] = {
            TEST_BLENDING_PAIR_VALUE_NAME(GL_FUNC_ADD),               // Default Blending function (index 0)
            TEST_BLENDING_PAIR_VALUE_NAME(GL_FUNC_SUBTRACT),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_FUNC_REVERSE_SUBTRACT),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_MIN),
            TEST_BLENDING_PAIR_VALUE_NAME(GL_MAX)
        };
        constexpr static inline std::size_t m_NumFuncs = sizeof(m_FuncValues) / sizeof(*m_FuncValues);

        int m_CurrentSRCFactorIdx;
        int m_CurrentDSTFactorIdx;
        int m_CurrentBlendFuncFuncIdx;

        void Update();
    };
}
