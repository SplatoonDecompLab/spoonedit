//
// Created by tv on 07.05.23.
//

#ifndef SPOONTOOL_SPECIALOBJECTPARAMS_H
#define SPOONTOOL_SPECIALOBJECTPARAMS_H

#include<yaml-cpp/yaml.h>

struct SpecialObjectParams {
public:
    SpecialObjectParams();

    virtual void ParseOut(YAML::Node node);

    virtual void InsertYaml(YAML::Emitter& Emitter);

    virtual ~SpecialObjectParams() = default;

    std::vector<std::string> GetMiscParamBlockList();
};

#endif //SPOONTOOL_SPECIALOBJECTPARAMS_H
