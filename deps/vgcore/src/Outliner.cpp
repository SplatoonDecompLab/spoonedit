#include<virintox/gcore/Outliner.h>
#include<imgui.h>
#include<iostream>

namespace Graphics {

    Outliner::Outliner() : Widget("Outliner",true){
        // NameEdit = NameEditor();

    }

    void Outliner::Draw() {
        if(MainItem == nullptr)
            return;
        DrawNode(MainItem);

    }

    void Outliner::DrawNode(ITreeSerializable* Node) {
        if(Node->GetObjectType() == Folder) {
            if (ImGui::TreeNodeEx(Node->GetName().c_str(), ImGuiTreeNodeFlags_SpanFullWidth)) {
                if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                    if(LastSelectedNode != nullptr)
                        LastSelectedNode->CurrentlySelected = false;
                    LastSelectedNode = Node;
                    Node->CurrentlySelected = true;
                }

                if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) ) {
                    NameEdit.Activate(Node);
                }

                for (auto item: Node->GetSubItems())
                    DrawNode(item);
                ImGui::TreePop();
                return;
            }


            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                if(LastSelectedNode != nullptr)
                    LastSelectedNode->CurrentlySelected = false;
                LastSelectedNode = Node;
                Node->CurrentlySelected = true;
            }

            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                NameEdit.Activate(Node);
            }
            return;

        }

        if(ImGui::TreeNodeEx(Node->GetName().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth)) {

            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                if(LastSelectedNode != nullptr)
                    LastSelectedNode->CurrentlySelected = false;
                LastSelectedNode = Node;
                Node->CurrentlySelected = true;
            }

            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                NameEdit.Activate(Node);
            }
            return;
        }
    }

    void Outliner::PostDraw() {
        Widget::PostDraw();
        if(NameEdit.Active) {
            NameEdit.PreDraw();
            NameEdit.Draw();
            NameEdit.PostDraw();
        }
    }



    NameEditorOld::NameEditorOld() : Widget("Outliner Name Editor") {
        std::cout << "Initializing Name Editor";
        return;
    }

    void NameEditorOld::PreDraw() {
        ImGui::Begin(Name.c_str(),&Active,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    }

    void NameEditorOld::Draw() {
        ImGui::Text("Change Name of Node to:");

        bool Done = ImGui::InputText("##",NewName,256,ImGuiInputTextFlags_EnterReturnsTrue);
        ImVec2 PrevPos = ImGui::GetCursorPos();
        Done |= ImGui::Button("Done",ImVec2(140.0f,30.0f));

        PrevPos.x += 145.0f;
        ImGui::SetCursorPos(PrevPos);
        Active &= !ImGui::Button("Cancel",ImVec2(140.0f,30.0f));
        if(Done){
            EditingNode->SetName(std::string(NewName));
            Active = false;
        }
    }
}

