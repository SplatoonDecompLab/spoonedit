//
// Created by tv on 09.05.23.
//

#ifndef SPOONTOOL_DEFAULT_H
#define SPOONTOOL_DEFAULT_H
#include<SpecialObjectParams.h>

class DefaultObjParams : public SpecialObjectParams {
public:
    DefaultObjParams() : SpecialObjectParams(){

    }

    void ParseOut(YAML::Node node) override {
        return;
    }

    void InsertYaml(YAML::Emitter &Emitter) override {

    }
};

#endif //SPOONTOOL_DEFAULT_H
