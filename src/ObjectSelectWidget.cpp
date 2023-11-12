//
// Created by tv on 21.05.23.
//

#include "ObjectSelectWidget.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "boost/filesystem.hpp"
#include<iostream>
#include<fstream>
#include"InstanceVars.h"
#include<virintox/gcore/Graphics.h>
#include "IconsFontAwesome6.h"

ObjectSelectWidget* ObjectSelectWidgetInstance = nullptr;

MENU_ITEM_DISPLAY(Widgets,Objects,ICON_FA_CUBES " Objects"){
    Graphics::window->WidgetByName.find("Objects")->second->Active ^= true;
}


ObjectSelectWidget::ObjectSelectWidget(): Graphics::Widget("Object Select",true) {

}

void ObjectSelectWidget::Draw() {
    static unsigned int counter = 0;
    counter = 0;

    for(auto iter = loadedMap.Objects.begin(); iter != loadedMap.Objects.end();iter++){
        LevelObject& obj = *iter;
        if(ImGui::Selectable((obj.Name + ": " + obj.Type + "##" + std::to_string(obj.runtimeID)).c_str(),selectedElem == &obj)){
            selectedElem = &obj;
        }

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered()){
            ImGui::OpenPopup((std::string("Right Click Menu: ObjSelect ") + std::to_string(obj.runtimeID)).c_str());
            //ImGui::OpenPopup((std::string("Change Name##: ObjSelect ") + (std::to_string(obj.runtimeID))).c_str());
        }
        if(ImGui::BeginPopup((std::string("Right Click Menu: ObjSelect ") + std::to_string(obj.runtimeID)).c_str())){
            std::string newmodal;

            if(ImGui::MenuItem((std::string("Delete##: ObjSelect ") + std::to_string(obj.runtimeID)).c_str())){
                std::cout << "Deleting " + obj.Name << std::endl;
                auto itertemp = iter;
                iter--;
                loadedMap.Objects.erase(itertemp);
                selectedElem = nullptr;
            }

            if(ImGui::MenuItem((std::string("Duplicate##: ObjSelect ") + std::to_string(obj.runtimeID)).c_str())){
                LevelObject dublObj = LevelObject(obj);
                dublObj.runtimeID = rand();

                loadedMap.Objects.emplace_back(dublObj);
                selectedElem = nullptr;
            }

            if(ImGui::MenuItem((std::string("Export##: ObjSelect ") + std::to_string(obj.runtimeID)).c_str())){
                YAML::Emitter emitter;

                obj.YamlInsert(emitter);

                auto fstr = std::ofstream((boost::filesystem::current_path() / "Presets" / (obj.Name + ".yaml")).string());

                fstr << emitter.c_str();
                fstr.flush();
                fstr.close();
            }

            ImGui::EndPopup();
        }

        counter++;
    }
}

ObjectSelectWidget* GetObjectSelectWidget(){
    return ObjectSelectWidgetInstance;
}

REGISTERVCLASS(ObjectSelectWidget)