//
// Created by tv on 28.06.23.
//

#include "RailSelectWidget.h"
#include <imgui.h>
#include "IconsFontAwesome6.h"
#include "MainViewport.h"
#include "InstanceVars.h"
#include<virintox/gcore/Graphics.h>
#include "IconsFontAwesome6.h"

MENU_ITEM_DISPLAY(Widgets,Rails,ICON_FA_CIRCLE_NODES " Rails"){
    Graphics::window->WidgetByName.find("Rails")->second->Active ^= true;
}

RailSelectWidget::RailSelectWidget(): Graphics::Widget("Rails",false) {

}

void RailSelectWidget::Draw() {
    for(auto railIter = loadedMap.Rails.begin();railIter != loadedMap.Rails.end();railIter++){
        Rail &rail = *railIter;
        if(ImGui::Selectable((rail.Name + ": " + rail.Type + "##" + std::to_string(rail.runtimeID)).c_str(), selectedElem == &rail)){
            selectedElem = &rail;
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
            ImGui::OpenPopup((std::string("Right Click Menu: RailSelect ") + std::to_string(rail.runtimeID)).c_str());
            //ImGui::OpenPopup((std::string("Change Name##: ObjSelect ") + (std::to_string(obj.runtimeID))).c_str());
        }
        if(ImGui::BeginPopup((std::string("Right Click Menu: RailSelect ") + std::to_string(rail.runtimeID)).c_str())){
            if(ImGui::MenuItem((std::string("Delete##: RailSelect ") + std::to_string(rail.runtimeID)).c_str())){
                std::cout << "Deleting " + rail.Name << std::endl;
                loadedMap.Rails.erase(railIter);
                selectedElem = nullptr;
                ImGui::EndPopup();
                break;
            }

            ImGui::EndPopup();
        }
    }

    if(ImGui::Button(ICON_FA_PLUS)){
        selectedElem = nullptr;

        auto vp = g_getMainViewport();
        Vector3 pos = {vp->camPos.x, -vp->camPos.y, vp->camPos.z};
        loadedMap.Rails.emplace_back(pos);
    }
}

REGISTERVCLASS(RailSelectWidget)