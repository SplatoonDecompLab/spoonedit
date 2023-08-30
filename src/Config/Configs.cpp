//
// Created by tv on 19.07.23.
//
#include"Configs.h"
#include<boost/filesystem.hpp>
#include<virintox/gcore/Graphics.h>

VGINIT_ACTION(LoadConfigs){
        Configs::g_loadConfigs("Gambit");
}

namespace Configs {
    static std::vector<std::string> loadStringListConfig(boost::filesystem::path path) {
        std::vector<std::string> cfg;

        YAML::Node Yaml = YAML::LoadFile(path.generic_string());

        for (auto elem: Yaml) {
            cfg.push_back(elem.as<std::string>());
        }

        return cfg;
    }

    template<typename t>
    static std::vector<ValueDisplayOptions<t>> loadValueDisplayConfig(boost::filesystem::path path) {
        std::vector<ValueDisplayOptions<t>> cfg;

        YAML::Node Yaml = YAML::LoadFile(path.generic_string());

        for (auto elem: Yaml) {
            cfg.push_back({elem["Val"].as<t>(), elem["Display"].as<std::string>()});
        }

        return cfg;
    }

    void g_loadConfigs(std::string game) {
        boost::filesystem::path gameCfgPath = boost::filesystem::current_path() / "Configs" / game;

        g_linkOpts = loadStringListConfig(gameCfgPath / "Links.yaml");
        g_areas = loadStringListConfig(gameCfgPath / "Areas.yaml");
        g_dropIdOpts = loadValueDisplayConfig<long>(gameCfgPath / "DropIds.yaml");
        g_teams = loadValueDisplayConfig<long>(gameCfgPath / "Teams.yaml");
        g_layers = loadValueDisplayConfig<std::string>(gameCfgPath / "Layers.yaml");
    }


}