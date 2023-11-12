//
// Created by tv on 29.06.23.
//

#ifndef SPOONTOOL_ELEMENT_H
#define SPOONTOOL_ELEMENT_H

#include<list>
#include<string>
#include"Transform.h"
#include"Link.h"

enum LayerConfig{
    Common, //Cmn
    Paint, //Pnt
    //Online, //Vss
    Rainmaker, //Vlf
    Tower, //Vgl
    Area, //Var
    Night, //Night
    Day, //Day
    Temporary //Tmp
};

class Element{
public:
    Element() = default;

    Element(const YAML::Node &ObjNode,std::list<std::string> PropertyBlockList = {});
    unsigned int runtimeID = static_cast<unsigned int>(rand());

    Transform TF = Transform();
    std::string Type = "Unnamed Type";
    std::string Name = "Unnamed Element";

    std::string ModelName = "null";

    std::string Layer = "Cmn";

    std::vector<int> Parameters = std::vector<int>();
    std::vector<float> FloatParameters = std::vector<float>();

    //std::map<std::string,std::vector<Link>> Links = std::map<std::string,std::vector<Link>>();

    std::vector<Link> Links = std::vector<Link>();
    std::map<std::string,std::string> OtherOptions = std::map<std::string,std::string>();

    bool IsLinkDest = false;
    void YamlInsert(YAML::Emitter &Emitter,bool compiled = false);

    virtual ~Element() = default;
protected:
    virtual void YamlInsertBody(YAML::Emitter &Emitter,bool compiled = false);
};

#endif //SPOONTOOL_ELEMENT_H