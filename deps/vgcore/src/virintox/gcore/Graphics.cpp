#include"Graphics.h"
#include"gui/Window.h"
#include"Shader.h"
#include"GLFW/glfw3.h"
#include"imgui.h"
#include"backends/imgui_impl_opengl3.h"
#include"backends/imgui_impl_glfw.h"
#include<stdexcept>
#include<iostream>
#include<Registries.h>



void ErrorCallback(GLenum source, GLenum type, GLuint id,
GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
    std::cout << "an opengl error occured";
}

namespace Graphics {
    VMIRRORASSEMBLY(VGCore, VMIRRORVERSION(0,1,0,0),VMirror::AssemblyType::StaticLibrary)

    VREGISTRY(Widget)
    VREGISTRY(MenuItem)
    VREGISTRY(MenuSortSetter)
    VREGISTRY(InitAction)

    void Init() {
        Running = true;
        if (!glfwInit()) {
            throw std::runtime_error("Could not Initialize glfw");
        }

        MWheel = 1.0f;
    }

    void BeginLoop() {
        while (Running) {
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

    void Terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }

    void EnableVsync() {
        glfwMakeContextCurrent(window->GetGlfwWindow());
        glfwSwapInterval(1);
    }

    void AutoInit(std::string name) {

        VMirror::getAssemblyRegistry()->selfRegister(getVGCoreAssemblyInfo());

        VMirror::getAssemblyRegistry()->flushQueue();

        Graphics::Init();
        Graphics::window = std::make_unique<Graphics::Window>(name);

        g_WidgetRegistry.collectObjects();

        for(const VMirror::ClassInfo* info: g_WidgetRegistry.m_matchedClasses){
            std::cout << "Adding Widget: " << info->m_name << std::endl;
            Graphics::window->addWidget(dynamic_cast<Graphics::Widget*>(info->construct()));
        }

        g_MenuItemRegistry.collectObjects();

        for(const VMirror::ClassInfo* info: g_MenuItemRegistry.m_matchedClasses){
            std::cout << "Adding Menu: " << info->m_name << std::endl;
            Graphics::window->addMenuItem(dynamic_cast<Graphics::MenuItem*>(info->construct()));
        }

        g_MenuSortSetterRegistry.collectObjects();

        for(const VMirror::ClassInfo* info: g_MenuSortSetterRegistry.m_matchedClasses){
            info->construct();
        }

        std::sort(Graphics::window->Menus.begin(), Graphics::window->Menus.end(),[](const Menu& menu1,const Menu& menu2){
            return menu1.m_sortId < menu2.m_sortId;
        });

        g_InitActionRegistry.collectObjects();

        for(const VMirror::ClassInfo* info: g_InitActionRegistry.m_matchedClasses){
            info->construct();
        }

        glfwSwapInterval(1);

        Graphics::BeginLoop();
        Graphics::Quit();
    }

    REGISTERABSTRACTVCLASS(InitAction)
}
