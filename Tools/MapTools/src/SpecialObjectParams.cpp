//
// Created by tv on 07.05.23.
//
#include "SpecialObjectParams.h"
#include"SpecialObjectParamVersions/All.h"


void SpecialObjectParams::ParseOut(YAML::Node node) {}

void SpecialObjectParams::InsertYaml(YAML::Emitter &Emitter) {}

SpecialObjectParams::SpecialObjectParams() {

}

std::vector<std::string> SpecialObjectParams::GetMiscParamBlockList() {
    return std::vector<std::string>();
}

