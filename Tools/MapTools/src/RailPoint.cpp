#include<Rail.h>
#include "RailPoint.h"


RailPoint::RailPoint(const YAML::Node &ObjNode): Element(ObjNode,{"ControlPoints","OffsetX","OffsetY","OffsetZ","UseOffset",}) {

    YAML::Node controlPointsNode = ObjNode["ControlPoints"];
    for(auto controlPoint: controlPointsNode){
        auto point = Vector3(controlPoint);
        m_controlPoints.push_back(point);
    }

    m_offset = {ObjNode["OffsetX"].as<float>(), ObjNode["OffsetY"].as<float>(), ObjNode["OffsetZ"].as<float>()};

    m_useOffset = ObjNode["UseOffset"].as<bool>();
}

RailPoint::RailPoint(const Vector3 &pos) {
    Name = "Unnamed Rail Point";
    Type = "Point";

    TF.Position = pos;
    m_controlPoints.push_back(pos);
    m_controlPoints.push_back(pos);
}

void RailPoint::YamlInsertBody(YAML::Emitter &Emitter,bool compiled) {
    Element::YamlInsertBody(Emitter);
    Emitter << YAML::Key << "ControlPoints" << YAML::BeginSeq;
    for(auto ctrlPoint: m_controlPoints){
        ctrlPoint.InsertIntoYaml(Emitter);
    }
    Emitter << YAML::EndSeq;

    Emitter << YAML::Key << "OffsetX" << YAML::Value << m_offset.X;
    Emitter << YAML::Key << "OffsetY" << YAML::Value << m_offset.Y;
    Emitter << YAML::Key << "OffsetZ" << YAML::Value << m_offset.Z;

    Emitter << YAML::Key << "UseOffset" << YAML::Value << m_useOffset;
}
