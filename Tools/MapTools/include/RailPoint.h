//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_RAILPOINT_H
#define SPOONTOOL_RAILPOINT_H

#include<Vector3.h>
#include<Transform.h>
#include<Link.h>

#include<yaml-cpp/yaml.h>
#include"Element.h"

struct RailPoint: public Element{
    RailPoint(const YAML::Node &ObjNode);
    explicit RailPoint(const Vector3& pos);

protected:

    void YamlInsertBody(YAML::Emitter &Emitter,bool compiled = false) override;
public:

    std::vector<Vector3> m_controlPoints = std::vector<Vector3>();

    Vector3 m_offset = {0,0,0};

    bool m_useOffset = false;

    ~RailPoint() override = default;
};

#endif //SPOONTOOL_RAILPOINT_H
