#include<virintox/gcore/EditorPopup.h>
#include<imgui.h>

namespace Graphics{
    NameEditor::NameEditor(std::function<void(std::string)> func,std::string name) : Widget(name) {
        SetFunc = func;
    }

    void NameEditor::PreDraw() {
        ImGui::Begin(Name.c_str(),&Active,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    }

    void NameEditor::Draw() {
        ImGui::Text("Change Name of Node to:");

        bool Done = ImGui::InputText("##",NewName,256,ImGuiInputTextFlags_EnterReturnsTrue);
        ImVec2 PrevPos = ImGui::GetCursorPos();
        Done |= ImGui::Button("Done",ImVec2(140.0f,30.0f));

        PrevPos.x += 145.0f;
        ImGui::SetCursorPos(PrevPos);
        Active &= !ImGui::Button("Cancel",ImVec2(140.0f,30.0f));
        if(Done){
            SetFunc(std::string(NewName));
            Active = false;
        }
    }
}