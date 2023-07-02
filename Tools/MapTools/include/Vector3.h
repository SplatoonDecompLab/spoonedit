
//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_VECTOR3_H
#define SPOONTOOL_VECTOR3_H

#include <yaml-cpp/yaml.h>
#include<boost/algorithm/string.hpp>
#include<iostream>

struct Vector3{

    Vector3(YAML::Node node);
    float X,Y,Z;

    inline Vector3(float x,float y,float z) : X(x),Y(y),Z(z){}

    inline void InsertIntoYaml(YAML::Emitter &Emitter){
        Emitter << YAML::BeginMap;
        Emitter << YAML::Key << "X" << YAML::Value << X;
        Emitter << YAML::Key << "Y" << YAML::Value << Y;
        Emitter << YAML::Key << "Z" << YAML::Value << Z;
        Emitter << YAML::EndMap;

    }
};
#endif //SPOONTOOL_VECTOR3_H