#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <iomanip>

namespace test
{
    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}
    };

    class TestMenu: public Test
    {
    public:
        TestMenu(Test*& currentTestPointer);

        void OnImGuiRender() override;

        template<typename T>
        void RegisterTest(const std::string& name)
        {
            std::cout << "Registering test " << std::quoted(name) << std::endl;
            m_Tests.emplace_back(name, []() { return new T{}; });
        }

        void UseTest(const std::string& name);

    private:
        Test*& m_CurrentTest;
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };
}
