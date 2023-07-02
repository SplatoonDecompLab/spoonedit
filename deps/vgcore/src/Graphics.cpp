#include<virintox/gcore/Graphics.h>
#include<virintox/gcore/Window.h>
#include<virintox/gcore/Shader.h>
#include<GLFW/glfw3.h>
#include<imgui.h>
#include<backends/imgui_impl_opengl3.h>
#include<backends/imgui_impl_glfw.h>
#include<stdexcept>
#include<iostream>

void ErrorCallback(GLenum source, GLenum type, GLuint id,
GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
    std::cout << "an opengl error occured";
}



void Graphics::Init() {
    Running = true;
    if (!glfwInit())
    {
        throw std::runtime_error("Could not Initialize glfw");
    }

    MWheel = 1.0f;
}

void Graphics::BeginLoop() {
    while(Running){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        window->Update();
        window->Draw();


        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(window->GetGlfwWindow());
    }
}

void Graphics::Terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

void Graphics::EnableVsync() {
    glfwSwapInterval(1);
}


