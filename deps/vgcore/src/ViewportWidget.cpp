#include<GL/glew.h>
#include<virintox/gcore/ViewportWidget.h>
#include<virintox/gcore/Shader.h>
#include<virintox/gcore/Viewport.h>
#include<imgui.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/string_cast.hpp>
#include<iostream>

namespace Graphics{
    ViewportWidget::ViewportWidget(std::string name,bool active) : Widget(name,active) , Viewport(){

    }

    void ViewportWidget::PreDraw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0,0));
        ImGui::Begin(Name.c_str(),&Active,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec2 windsize = ImGui::GetWindowSize();

        framewidth = windsize.x;
        frameheight = windsize.y - 23;

        if((framewidth != lastframewidth) || (frameheight != lastframeheight))
            RecreateFramebuffer();
        glBindFramebuffer(GL_FRAMEBUFFER,Framebuffer);
        glViewport(0,0,framewidth,frameheight);
        glClearColor(.2,.2,.2,1);
        glClear(GL_COLOR_BUFFER_BIT   | GL_DEPTH_BUFFER_BIT);



    }

    void ViewportWidget::PostDraw() {

        auto preimgcpos = ImGui::GetCursorPos();

        ImVec2 windsize = ImGui::GetWindowSize();
        windsize.y -= 23;

        //glFinish();

        ImGui::SetCursorPos(ImVec2(0,23));
        ImGui::Image(reinterpret_cast<ImTextureID>(FramebufferTex),windsize);

        if(ImGui::IsItemHovered()){

            std::vector<InputEvent> Events;
            ImGui::SetCursorPos(ImVec2(0,23));

            ImVec2 ScreenPos = ImGui::GetMousePos();
            ImVec2 PosRel = ImGui::GetCursorScreenPos();

            float X = ScreenPos.x - PosRel.x;
            float Y = ScreenPos.y - PosRel.y;

            X /= windsize.x;
            Y /= windsize.y;

            InputEvent Event = InputEvent {X,Y,MouseHover};
            Events.push_back(Event);
            ImGuiIO& IO = ImGui::GetIO();

            if(ImGui::IsMouseClicked(ImGuiMouseButton_Right)){
                InputEvent NEvent = Event;
                NEvent.EventType = MouseDownR;
                Events.push_back(NEvent);
            }
            if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
                InputEvent NEvent = Event;
                NEvent.EventType = MouseDownL;
                Events.push_back(NEvent);
            }


            if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)){
                InputEvent NEvent = Event;
                NEvent.EventType = MouseUpR;
                Events.push_back(NEvent);
            }
            if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)){
                InputEvent NEvent = Event;
                NEvent.EventType = MouseUpL;
                Events.push_back(NEvent);
            }

            if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                InputEvent NEvent = Event;
                NEvent.EventType = MouseHoldR;
                Events.push_back(NEvent);
            }
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)){
                InputEvent NEvent = Event;
                NEvent.EventType = MouseHoldL;
                Events.push_back(NEvent);
            }

            if(IO.MouseWheel != 0){
                InputEvent NEvent = Event;
                NEvent.EventType = MWheel;
                NEvent.MWheel = IO.MouseWheel;
                Events.push_back(NEvent);
            }

            for(InputEvent e: Events) {
                HandleInput(e);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER,0);
        ImGui::PopStyleVar();

        ImGui::SetCursorPos(preimgcpos);
        DrawOver();

        ImGui::End();

    }

    void ViewportWidget::DrawOver() {}
}