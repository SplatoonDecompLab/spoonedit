#include<GL/glew.h>
#include"Window.h"
#include"MessageBox.h"
#include"../Graphics.h"
#include"../Shader.h"
#include <exception>
#include"imgui.h"
#include"backends/imgui_impl_glfw.h"
#include"backends/imgui_impl_opengl3.h"
#include<stdexcept>
#include<iostream>
#include "IconsFontAwesome6.h"


void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    Graphics::MWheel -= yoffset * Graphics::MWheel / 2;
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    std::vector<std::string> Files = std::vector<std::string>();
    for (int x = 0;  x < count; x++)
        Files.push_back(std::string(paths[x]));
    Graphics::window->DropCallback(Files);
}

namespace Graphics {
    Window::Window(const std::string &name) {
        Widgets = std::vector<std::unique_ptr<Widget>>();

        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 6 );
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

        window = glfwCreateWindow(900,900,name.c_str(),NULL,NULL);
        glfwMakeContextCurrent(window);
        glfwSetScrollCallback(window,ScrollCallback);
        glfwSetDropCallback(window, drop_callback);
        glewExperimental = true;
        if(GLEW_OK != glewInit())
            throw std::runtime_error("Could not Initialize glew");

        glfwSwapInterval(0);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.WantCaptureKeyboard = false;
        io.WantCaptureMouse = false;
        io.WantCaptureMouseUnlessPopupClose = false;

        io.Fonts->AddFontFromFileTTF("Roboto-Regular.ttf",16);

        float baseFontSize = 16.0f;
        float iconFontSize = baseFontSize * 2.0f / 3.0f;

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = iconFontSize;
        io.Fonts->AddFontFromFileTTF( FONT_ICON_FILE_NAME_FAS, iconFontSize, &icons_config, icons_ranges );

        ImGuiStyle &style = ImGui::GetStyle();
        style.FrameRounding = 4;
        style.WindowRounding = 12;
        style.ChildBorderSize = 1.0f;
        style.WindowTitleAlign = ImVec2(0.5f,0.5f);
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        ImGui::StyleColorsDark();
    }

    void Window::Update() {



    }

    void Window::Draw() {
        if(glfwWindowShouldClose(window)) {
            glfwSetWindowShouldClose(window,GLFW_FALSE);
            ImGui::OpenPopup("Are you sure?##QuitModal");
        }

        glfwSwapInterval(0);
        DrawBG();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(),ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::BeginMainMenuBar();
        for(auto &menu: Menus) {
            if(ImGui::BeginMenu(menu.m_name.c_str())) {
                for (auto &item: menu.m_items) {
                    item->draw();
                }

                ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();


        for(auto& widget: Widgets) {
            if(widget->Active) {
#ifdef NDEBUG
                try{
#endif
                    widget->PreDraw();
                    widget->Draw();
                    widget->PostDraw();
#ifdef NDEBUG
                } catch(const MSGBoxException& msgexc){
                    std::cout << msgexc.what() << std::endl;
                    msgBoxes.push_back(new MessageBox(msgexc.whattext,msgexc.Severity));
                    ImGui::End();
                } catch(const std::exception& exc){
                    std::cout << exc.what() << std::endl;
                    msgBoxes.push_back(new MessageBox(exc.what(),Fault));
                    ImGui::End();
                } catch(...){
                    std::cout << "Something happend .-." << std::endl;
                    msgBoxes.push_back(new MessageBox("An unknown error occured",Fault));
                    ImGui::End();
                }
#endif
            }
        }

        for(auto tempWidgetIter = TemporaryWidgets.begin(); tempWidgetIter != TemporaryWidgets.end(); tempWidgetIter++){
            if ((*tempWidgetIter)->Active == true){
                (*tempWidgetIter)->PreDraw();
                (*tempWidgetIter)->Draw();
                (*tempWidgetIter)->PostDraw();
            }
        }

        for(auto box :msgBoxes){
            bool brk;
            box->PreDraw();
            box->Draw();
            brk = box->Deactivate;
            box->PostDraw();
            if(brk) break;
        }

        if(ImGui::BeginPopupModal("Are you sure?##QuitModal",NULL,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)){
            ImGui::Text("Are you sure that you wish to quit?");
            ImGui::SetCursorPosX(ImGui::CalcTextSize("Are you sure that you wish to quit?").x - ImGui::CalcTextSize(" Yes No ").x);
            if(ImGui::Button("Yes"))
                Graphics::Quit();
            ImGui::SameLine();
            if(ImGui::Button("No"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void Window::DropCallback(const std::vector<std::string> &paths) {}

    void Window::DrawBG() {
        glClearColor(.2,.2,.2,1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    MenuSortSetter::MenuSortSetter(std::string menuName, unsigned int id) {
        auto menuIter = std::find_if(Graphics::window->Menus.begin(), Graphics::window->Menus.end(),[&](const Menu &menu){
            return menu.m_name == menuName;
        });

        menuIter->m_sortId = id;
    }

    REGISTERABSTRACTVCLASS(MenuSortSetter)
}

