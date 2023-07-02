//
// Created by tv on 28.06.23.
//

#include "RailSelectWidget.h"
#include "MainWindow.h"
#include <imgui.h>
#include "IconsFontAwesome6.h"
#include "MainViewport.h"

RailSelectWidget::RailSelectWidget(): Graphics::Widget("Rails",false) {

}

void RailSelectWidget::Draw() {
    auto wind = GetMainWindow();

    for(auto railIter = wind->loadedMap.Rails.begin();railIter != wind->loadedMap.Rails.end();railIter++){
        Rail &rail = *railIter;
        if(ImGui::Selectable((rail.Name + ": " + rail.Type + "##" + std::to_string(rail.runtimeID)).c_str(), wind->selectedElem == &rail)){
            wind->selectedElem = &rail;
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
            ImGui::OpenPopup((std::string("Right Click Menu: RailSelect ") + std::to_string(rail.runtimeID)).c_str());
            //ImGui::OpenPopup((std::string("Change Name##: ObjSelect ") + (std::to_string(obj.runtimeID))).c_str());
        }
        if(ImGui::BeginPopup((std::string("Right Click Menu: RailSelect ") + std::to_string(rail.runtimeID)).c_str())){
            if(ImGui::MenuItem((std::string("Delete##: RailSelect ") + std::to_string(rail.runtimeID)).c_str())){
                std::cout << "Deleting " + rail.Name << std::endl;
                wind->loadedMap.Rails.erase(railIter);
                wind->selectedElem = nullptr;
                ImGui::EndPopup();
                break;
            }

            ImGui::EndPopup();
        }
    }

    if(ImGui::Button(ICON_FA_PLUS)){
        wind->selectedElem = nullptr;

        auto vp = GetMainViewport();
        Vector3 pos = {vp->camPos.x, -vp->camPos.y, vp->camPos.z};
        wind->loadedMap.Rails.emplace_back(pos);
    }
}
