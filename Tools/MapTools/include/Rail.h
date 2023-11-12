//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_RAIL_H
#define SPOONTOOL_RAIL_H

#include<Transform.h>
#include<Link.h>
#include<RailPoint.h>

#include<yaml-cpp/yaml.h>
#include<RailPoint.h>
#include "Element.h"

enum RailType{
    Linear,
    Bezier
};

inline std::string ToString(RailType railType){
    if(railType == Linear){
        return "Linear";
    } else if(railType == Bezier){
        return "Bezier";
    }
}

inline RailType ToRailType(std::string railType){
    if(railType == "Linear"){
        return Linear;
    } else if(railType == "Bezier"){
        return Bezier;
    }
}

constexpr std::array<RailType,2> AllRailTypeOptions = {Linear,Bezier};

struct Rail: public Element{

    Rail(const YAML::Node &ObjNode);
    explicit Rail(const Vector3 &Position);

    std::list<RailPoint> Points = std::list<RailPoint>();

    RailType m_railType = RailType::Linear;
    bool IsClosed = false;
    bool IsLadder = false;
    long Priority = 100;

    ~Rail() override = default;

protected:
    void YamlInsertBody(YAML::Emitter &Emitter,bool compiled = false) override;
};

#endif //SPOONTOOL_RAIL_H
