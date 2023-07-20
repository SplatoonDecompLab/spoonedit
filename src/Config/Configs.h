//
// Created by tv on 19.07.23.
//

#ifndef SPOONTOOL_CONFIGS_H
#define SPOONTOOL_CONFIGS_H

#include<string>
#include<yaml-cpp/yaml.h>
#include<imgui.h>
#include<algorithm>

namespace Configs {
    void g_loadConfigs(std::string game);

    template<typename t>
    struct ValueDisplayOptions{
        t m_val;
        std::string m_display;
    };

    inline void imguiDrawOpts(std::string id,std::vector<std::string> opts,std::string &val){
        if(ImGui::BeginCombo(id.c_str(),val.c_str())){
             for(const std::string &opt: opts) {
                 if (ImGui::Selectable((opt + "##" + id).c_str()))
                     val = opt;
             }

            ImGui::EndCombo();
        }
    }

    template<typename t>
    inline void imguiDrawOpts(std::string id,std::vector<ValueDisplayOptions<t>> opts,t &val){
        auto selOpt = std::find_if(opts.begin(), opts.end(),[&](const ValueDisplayOptions<t> &opt){
            return val == opt.m_val;
        });

        if(ImGui::BeginCombo(id.c_str(),selOpt->m_display.c_str())){
            for(const auto &opt: opts) {
                if (ImGui::Selectable((opt.m_display + "##" + id).c_str()))
                    val = opt.m_val;
            }

            ImGui::EndCombo();
        }
    }

    inline std::vector<std::string> g_linkOpts;
    inline std::vector<std::string> g_areas;
    inline std::vector<ValueDisplayOptions<long>> g_dropIdOpts;



}


#endif //SPOONTOOL_CONFIGS_H
