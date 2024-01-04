#include "Test.h"

#include <imgui.h>

namespace test
{
    TestMenu::TestMenu(Test*& currentTestPointer):
        m_CurrentTest{currentTestPointer}
    {}

    void TestMenu::OnImGuiRender()
    {
        for(auto& test : m_Tests)
        {
            if(ImGui::Button(test.first.c_str()))
            {
                m_CurrentTest = test.second();
                break;
            }
        }
    }

    void TestMenu::UseTest(const std::string& name)
    {
        if(name == "")
        {
            return;
        }
        for(auto& test : m_Tests)
        {
            if(name == test.first)
            {
                std::cout << "Using test " << std::quoted(name) << std::endl;
                m_CurrentTest = test.second();
                return;
            }
        }
        std::cout << "Test " << std::quoted(name) << " was not found!" << std::endl;
    }
}
