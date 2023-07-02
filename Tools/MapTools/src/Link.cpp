//
// Created by tv on 30.04.23.
//
#include<Link.h>

Link::Link(const YAML::Node &ObjNode) {
    Name = ObjNode["DefinitionName"].as<std::string>();
    Destination = ObjNode["DestUnitId"].as<std::string>();
    UnitFile = ObjNode["UnitFileName"].as<std::string>();
}

void Link::InsertIntoYaml(YAML::Emitter &Emitter) {
    Emitter << YAML::BeginMap;
    Emitter << YAML::Key << "DefinitionName" << YAML::Value << Name;
    Emitter << YAML::Key << "DestUnitId" << YAML::Value << Destination;
    Emitter << YAML::Key << "UnitFileName" << YAML::Value << UnitFile;
    Emitter << YAML::EndMap;
}
