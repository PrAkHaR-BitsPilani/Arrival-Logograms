#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <vector>

#include "VertexArray.h"
#include "Shader.h"

using namespace std;

#define point struct point
point{
    float x,y;
};

#define triangle struct triangle
triangle{
    point a , b , c;
};

float getRandomFloat(float theta)
{
    //if (theta > 300.0f)
        return ((float)rand() / RAND_MAX) * 10.0f;
    return 1.0f;
}

vector<triangle> getLogogram(float diff , int& indexCount)
{
    vector<triangle>logogram;

    float theta = 0;
    float maxHeight = 0.01f;
    point a0 = { 0.5f , 0 };
    point a1 = { 0.5f + getRandomFloat(theta) * maxHeight, 0 };
    theta += diff;
    while (theta <= 360.0f)
    {
        point b0 = { 0.5f * cos(glm::radians(theta)),0.5f * sin(glm::radians(theta)) };
        float newRadius = 0.5f + getRandomFloat(theta) * maxHeight;
        point b1 = { newRadius * cos(glm::radians(theta)), newRadius * sin(glm::radians(theta)) };
        logogram.push_back({ a0 , a1 , b1 });
        logogram.push_back({ a0 , b1 , b0 });
        indexCount += 6;
        a0 = b0;
        a1 = b1;
        theta += diff;
    }
    if (theta > 360.0f) {
        theta = 360.0f;
        point b0 = { 0.5f * cos(glm::radians(theta)),0.5f * sin(glm::radians(theta)) };
        float newRadius = 0.5f + getRandomFloat(theta) * maxHeight;
        point b1 = { newRadius * cos(glm::radians(theta)), newRadius * sin(glm::radians(theta)) };
        logogram.push_back({ a0 , a1 , b1 });
        logogram.push_back({ a0 , b1 , b0 });
        indexCount += 6;
    }
    


    return logogram;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 800, "Prakhar6969", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // glad: load all OpenGL function pointers
    // ---------------------------------------

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int count = 0;
    float diff = 10;
    auto points = getLogogram(diff , count);

    VertexArray* VAO = new VertexArray();
    VertexBuffer* VBO = new VertexBuffer(points.data(), points.size() * sizeof(triangle));
    VertexBufferLayout* layout = new VertexBufferLayout();
    layout->push<float>(2);
    VAO->addBuffer(*VBO, *layout);
    VAO->bind();

    Shader* shader = new Shader("res/shaders/basic.shader");
    shader->bind();

    //glPointSize(2);

    //Setup IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1, 1, 1, 1);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SliderFloat("Difference", &diff, 0.1, 360);
        ImGui::Spacing();

        points = getLogogram(diff, count);
        VBO->updateData(points.data(), points.size() * sizeof(triangle));

        glDrawArrays(GL_TRIANGLES, 0, count);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Shutdown IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}