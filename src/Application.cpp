#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
            -50.0f, -50.0f, 0.0f, 0.0f, // 0
            +50.0f, -50.0f, 1.0f, 0.0f, // 1
            +50.0f, +50.0f, 1.0f, 1.0f, // 2
            -50.0f, +50.0f, 0.0f, 1.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        VertexArray va;

        VertexBuffer vb{positions, sizeof(positions)};
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib{indices, (sizeof(indices)/sizeof(indices[0]))};
        va.AddIndexBuffer(ib);

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        Shader shader{"", "Basic"};

        Texture texture{"", "Pie"};
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();

        ImGui::StyleColorsDark();

        glm::vec3 translationA(200.0f, 200.0f, 0.0f);
        glm::vec3 translationB(400.0f, 400.0f, 0.0f);

        glm::vec4 clear_color(0.0f, 1.0f, 1.0f, 1.0f);
        shader.SetUniform4f("u_Color", clear_color.r, clear_color.g, clear_color.b, clear_color.a);

        /* Loop until the user closes the window */
        while(!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow(); // Show demo window! :)

            ImGui::Begin("Hello, viewport!");

            ImGui::ColorEdit4("Clear color", (float*)&clear_color);
            shader.SetUniform4f("u_Color", clear_color.r, clear_color.g, clear_color.b, clear_color.a);

            ImGui::SliderFloat2("Model position A", (float*)&translationA.x, 0.0f, 600.0f);
            ImGui::SliderFloat2("Model position B", (float*)&translationB.x, 0.0f, 600.0f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

            shader.Bind();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, shader);
            }

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, shader);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
