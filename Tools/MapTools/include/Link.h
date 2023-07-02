//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_LINK_H
#define SPOONTOOL_LINK_H

#include <yaml-cpp/yaml.h>
#include<string>

constexpr std::array<const char*, 12> LinkTypes = {
        "Spawner",
        "Rail",
        "Area",
        "AutoWarpPointLink",
        "Switch",
        "SwitchSender",
        "LiftBindable",
        "GraphNode",
        "ObjToGraphNode",
        "ObjToGraphNodeOneway",
        "BoneBindable",
        "ToGachihokoTargetPoint",
};

struct Link {
    inline Link() : Name("Rail"), Destination("Undefined Destination"), UnitFile("") {}

    Link(const YAML::Node &ObjNode);

    // DefinitionName in byaml
    std::string Name;

    // DestUnitId in byaml
    std::string Destination;

    // UnitFileName in byaml
    static constexpr char LinkUnitFileID[] = "UnitFile";
    std::string UnitFile;

    void InsertIntoYaml(YAML::Emitter &Emitter);

};

#endif //SPOONTOOL_LINK_H
