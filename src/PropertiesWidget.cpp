//
// Created by tv on 21.05.23.
//

#include "PropertiesWidget.h"
#include "MainWindow.h"
#include<imgui.h>
#include "misc/cpp/imgui_stdlib.h"
#include "IconsFontAwesome6.h"
#include<array>
#include "Config/Configs.h"

struct SimpleVec3 {
    float x, y, z;

    template<typename t>
    SimpleVec3(t fromobj) {
        x = fromobj.X;
        y = fromobj.Y;
        z = fromobj.Z;
    }

    template<typename t>
    void copyto(t *toobj) {
        toobj->X = x;
        toobj->Y = y;
        toobj->Z = z;
    }
};

PropertiesWidget *PropertiesWidgetInst = nullptr;

template<typename t, typename iter, typename stringConv>
void ImGuiDrawSelection(std::string id, t &val, iter optsBegin, iter optsEnd, stringConv convert) {

    if (ImGui::BeginCombo(id.c_str(), convert(val).c_str())) {
        while (optsBegin != optsEnd) {
            if (ImGui::Selectable((convert(*optsBegin) + "##" + id).c_str()))
                val = *optsBegin;
            optsBegin++;
        }
        ImGui::EndCombo();
    }

}

template<typename t>
void ImGuiDrawLayer(t *obj) {
    if (ImGui::BeginCombo("Layer##PropWind", ToString(obj->Layer).c_str())) {
        if (ImGui::Selectable("Common##TeamsPropWind"))
            obj->Layer = Common;
        if (ImGui::Selectable("Paint##TeamsPropWind"))
            obj->Layer = Paint;
        if (ImGui::Selectable("Rainmaker##TeamsPropWind"))
            obj->Layer = Rainmaker;
        if (ImGui::Selectable("Tower##TeamsPropWind"))
            obj->Layer = Tower;
        if (ImGui::Selectable("Area##TeamsPropWind"))
            obj->Layer = Area;
        if (ImGui::Selectable("Night##TeamsPropWind"))
            obj->Layer = Night;
        if (ImGui::Selectable("Day##TeamsPropWind"))
            obj->Layer = Day;
        if (ImGui::Selectable("Temporary##TeamsPropWind"))
            obj->Layer = Temporary;
        ImGui::EndCombo();
    }
};

void ImGuiDrawTransform(Transform &tf, std::string id) {
    ImGui::Text("Transform:");
    ImGui::Indent();
    {
        //auto simplevec3pos = SimpleVec3(tf.Position);
        ImGui::DragFloat3(("Position##" + id).c_str(), &(tf.Position.X), 0.01);
        //simplevec3pos.copyto(&tf.Position);

        //auto simplevec3rot = SimpleVec3(tf.Rotation);
        ImGui::DragFloat3(("Rotation##" + id).c_str(), &(tf.Rotation.X), 0.01);
        //simplevec3rot.copyto(&tf.Rotation);

        //auto simplevec3scale = SimpleVec3(tf.Scale);
        ImGui::DragFloat3(("Scale##" + id).c_str(), &(tf.Scale.X), 0.01);
        //simplevec3scale.copyto(&tf.Scale);
    }
    ImGui::Unindent();
}

void ImGuiDrawElem(Element *elem, std::string Id = "") {
    Rail *rail = dynamic_cast<Rail *>(elem);
    RailPoint *railPoint = dynamic_cast<RailPoint *>(elem);
    LevelObject *levelObject = dynamic_cast<LevelObject *>(elem);

    Id += std::to_string(elem->runtimeID);

    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 3);
    ImGui::InputText(("##PropWindInputName" + Id).c_str(), &elem->Name);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text(": ");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() / 3);
    ImGui::InputText(("##PropWindInputType" + Id).c_str(), &elem->Type);
    ImGui::PopItemWidth();
    ImGuiDrawTransform(elem->TF, "PropWind" + Id);
    ImGuiDrawLayer(elem);
    ImGui::Checkbox(("Is Link Destination?##PropWindowIsLinkDest" + Id).c_str(), &elem->IsLinkDest);
    if (ImGui::CollapsingHeader(("Links##" + Id).c_str())) {
        ImGui::Indent();
        unsigned i = 1;
        for (auto linkIter = elem->Links.begin(); linkIter != elem->Links.end(); linkIter++) {
            auto &link = *linkIter;
            if (ImGui::CollapsingHeader(("Link " + std::to_string(i) + "##" + Id).c_str())) {
                ImGui::Indent();
                Configs::g_imguiDrawOpts("Link Type##" + std::to_string(i) + Id, Configs::g_linkOpts, link.Name);
                ImGui::InputText(("Destination##PropWindLink" + std::to_string(i) + Id).c_str(), &link.Destination);

                if (Element *gotoElem = GetMainWindow()->loadedMap.GetElementById(link.Destination)) {
                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FA_CHEVRON_RIGHT)) {
                        GetMainWindow()->selectedElem = gotoElem;
                    }
                }

                ImGui::InputText(("Unit File##PropWindLink" + std::to_string(i) + Id).c_str(), &link.UnitFile);
                if (ImGui::Button((ICON_FA_TRASH "##PropWindLinkDelete" + std::to_string(i) + Id).c_str())) {
                    elem->Links.erase(linkIter);
                    ImGui::Unindent();
                    break;
                }
                ImGui::Unindent();
            }
            i++;
        }

        if (ImGui::Button(ICON_FA_PLUS)) {
            elem->Links.emplace_back();
        }
        ImGui::Unindent();
    }
    if (ImGui::CollapsingHeader(("Element Type Specific Options##" + Id).c_str())) {
        ImGui::Indent();
        if (rail) {
            ImGui::Checkbox(("Is Closed?##" + Id).c_str(), &rail->IsClosed);
            ImGui::Checkbox(("Is Ladder?##" + Id).c_str(), &rail->IsLadder);
            ImGui::InputInt(("Priority?##" + Id).c_str(), (int *) &rail->Priority);
            ImGuiDrawSelection("Rail Type", rail->RailType, AllRailTypeOptions.begin(), AllRailTypeOptions.end(),
                               [](RailType t) { return ToString(t); });
            if (ImGui::CollapsingHeader(("Rail Points##" + Id).c_str())) {
                ImGui::Indent();
                unsigned int i = 1;

                for (auto curRailPointRef = rail->Points.begin();
                     curRailPointRef != rail->Points.end(); curRailPointRef++) {
                    RailPoint &curRailPoint = *curRailPointRef;
                    if (ImGui::CollapsingHeader((std::to_string(i) + "##RailPoints").c_str())) {
                        ImGui::Indent();
                        ImGuiDrawElem(&curRailPoint,
                                      "RailPoint" + std::to_string(i) + std::to_string(curRailPoint.runtimeID));
                        if (ImGui::Button(
                                (ICON_FA_TRASH "##PropWindRailPointDelete" + std::to_string(i) + Id).c_str())) {
                            rail->Points.erase(curRailPointRef);
                            ImGui::Unindent();
                            break;
                        }
                        ImGui::Unindent();
                    }
                    i++;
                }
                if (ImGui::Button((ICON_FA_PLUS "##RailPointAdd" + Id).c_str())) {
                    RailPoint rp = RailPoint(rail->Points.back());
                    rp.runtimeID = rand();
                    rail->Points.push_back(rp);
                }

                if (rail->Points.empty()) {
                    auto &map = GetMainWindow()->loadedMap;

                    std::erase_if(map.Rails, [&](const Rail &rail1) {
                        return rail1.runtimeID == rail->runtimeID;
                    });

                    GetMainWindow()->selectedElem = nullptr;
                }
                ImGui::Unindent();
            }
        }

        if (levelObject) {
            //ImGuiDrawTeamSelect(levelObject,Id);
            Configs::g_imguiDrawOpts("Team##" + Id, Configs::g_teams, levelObject->Team);

            Configs::g_imguiDrawOpts("Drop##" + Id, Configs::g_dropIdOpts, levelObject->DropId);
        }

        if (railPoint) {
            ImGui::Checkbox(("Use Offset##" + Id).c_str(), &railPoint->m_useOffset);
            if (railPoint->m_useOffset) {
                ImGui::Indent();
                ImGui::DragFloat3(("Offset##" + Id).c_str(), &(railPoint->m_offset.X), 0.01);
                ImGui::Unindent();
            }

            unsigned i = 0;
            for (Vector3 &ctrlPoint: railPoint->m_controlPoints) {
                ImGui::DragFloat3(("Control point " + std::to_string(i) + "##" + Id).c_str(), &(ctrlPoint.X), 0.01);
                i++;
            }

        }
        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader(("Parameters##" + Id).c_str())) {
        ImGui::Indent();
        for (int i = 0; i < elem->Parameters.size(); i++) {
            ImGui::InputInt(("Parameter " + std::to_string(i) + "##PropWind" + Id).c_str(),
                            (int *) &elem->Parameters[i]);
            ImGui::SameLine();
            if (ImGui::Button((ICON_FA_TRASH "##ParamDelete" + std::to_string(i) + "th" + Id).c_str())) {
                auto delparam = elem->Parameters.begin() + i;

                elem->Parameters.erase(delparam);
                break;
            }
        }
        if (ImGui::Button((ICON_FA_PLUS "##ParamAdd" + Id).c_str())) {
            elem->Parameters.push_back(-99);
        }

        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader(("Float Parameters##" + Id).c_str())) {
        ImGui::Indent();
        for (int i = 0; i < elem->FloatParameters.size(); i++) {
            ImGui::InputFloat(("Parameter " + std::to_string(i) + "##FloatParamPropWind" + Id).c_str(),
                              &elem->FloatParameters[i]);
            ImGui::SameLine();
            if (ImGui::Button((ICON_FA_TRASH "##FloatParamDelete" + std::to_string(i) + "th" + Id).c_str())) {
                auto delparam = elem->FloatParameters.begin() + i;

                elem->FloatParameters.erase(delparam);
                break;
            }
        }

        if (ImGui::Button((ICON_FA_PLUS "##FloatParamAdd" + Id).c_str())) {
            elem->FloatParameters.push_back(-99.0f);
        }
        ImGui::Unindent();
    }

    if (!elem->OtherOptions.empty()) {
        if (ImGui::CollapsingHeader(("Miscellaneous Options##" + Id).c_str())) {
            ImGui::Indent();
            for (auto &opt: elem->OtherOptions) {
                ImGui::Text("%s:", opt.first.c_str());
                ImGui::SameLine();
                ImGui::InputText(("##" + opt.first + Id).c_str(), &opt.second);
            }
            ImGui::Unindent();
        }
    }
}

PropertiesWidget::PropertiesWidget() : Graphics::Widget("Properties", true) {
    PropertiesWidgetInst = this;

    GetMainWindow()->selectedElem = nullptr;
}

void PropertiesWidget::Draw() {
    auto wind = GetMainWindow();

    if (wind->selectedElem != nullptr) {
        ImGuiDrawElem(wind->selectedElem);
    }
}

PropertiesWidget *GetPropertiesWidget() {
    return PropertiesWidgetInst;
}