//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_TRANSFORM_H
#define SPOONTOOL_TRANSFORM_H
#include<Vector3.h>
#include<yaml-cpp/yaml.h>

struct Transform{

    inline Transform(): Position(0,0,0), Scale(1,1,1), Rotation(0,0,0) {}

    Vector3 Position,Scale,Rotation;

    void InsertIntoYaml(YAML::Emitter &Emitter);
};

#endif //SPOONTOOL_TRANSFORM_H
