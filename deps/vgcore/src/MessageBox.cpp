#include<virintox/gcore/Graphics.h>
#include<virintox/gcore/MessageBox.h>
#include <imgui.h>
#include <string>


unsigned int errorcount = 0;

namespace Graphics{
    const char * MSGBoxException::what() const noexcept {
        return whattext.c_str();
    }

    MessageBox::MessageBox(std::string Text, ErrorSeverity Severity): Widget(std::to_string(errorcount)), text(Text),severity(Severity){
        errorcount++;
    }

    void MessageBox::Draw(){
        switch(severity){
            case Fault:
                ImGui::Text("Fault:");
            break;
            case Error:
                ImGui::Text("Error:");
            break;
            case Warning:
                ImGui::Text("Warning:");
            break;
            case Info:
                ImGui::Text("Info:");
            break;
        }

        ImGui::Text(text.c_str());

        if(ImGui::Button("Close"))
            Deactivate = true;
    }

    void MessageBox::PostDraw(){
        Widget::PostDraw();
        if(Deactivate)
            delete this;
    }

    MessageBox::~MessageBox(){
        window->msgBoxes.remove(this);
    }
}