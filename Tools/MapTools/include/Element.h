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

inline std::string ToStringYaml(LayerConfig cfg){
    if(cfg == Common){
        return "Cmn";
    } else if(cfg == Paint){
        return "Pnt";
    } else if(cfg == Rainmaker) {
        return "Vlf";
    } else if(cfg == Tower) {
        return "Vgl";
    } else if(cfg == Area) {
        return "Var";
    } else if(cfg == Night) {
        return "Night";
    } else if(cfg == Day) {
        return "Day";
    } else if(cfg == Temporary) {
        return "Tmp";
    }
}

inline std::string ToString(LayerConfig cfg){
    if(cfg == Common){
        return "Common";
    } else if(cfg == Paint){
        return "Paint";
    } else if(cfg == Rainmaker) {
        return "Rainmaker";
    } else if(cfg == Tower) {
        return "Tower";
    } else if(cfg == Area) {
        return "Area";
    } else if(cfg == Night) {
        return "Night";
    } else if(cfg == Day) {
        return "Day";
    } else if(cfg == Temporary) {
        return "Temporary";
    }
}

inline LayerConfig ToLayerConfig(std::string cfg){
    if(cfg == "Cmn"){
        return Common;
    } else if(cfg == "Pnt"){
        return Paint;
    } else if(cfg == "Vlf") {
        return Rainmaker;
    } else if(cfg == "Vgl") {
        return Tower;
    } else if(cfg == "Var") {
        return Area;
    } else if(cfg == "Night") {
        return Night;
    } else if(cfg == "Day") {
        return Day;
    } else if(cfg == "Tmp") {
        return Temporary;
    }
}

class Element{
public:
    Element() = default;

    Element(const YAML::Node &ObjNode,std::list<std::string> PropertyBlockList = {});
    unsigned int runtimeID = static_cast<unsigned int>(rand());

    Transform TF = Transform();
    std::string Type = "Unnamed Type";
    std::string Name = "Unnamed Element";

    std::string ModelName = "null";

    LayerConfig Layer = Common;

    std::vector<long> Parameters = std::vector<long>();
    std::vector<float> FloatParameters = std::vector<float>();

    //std::map<std::string,std::vector<Link>> Links = std::map<std::string,std::vector<Link>>();

    std::vector<Link> Links = std::vector<Link>();
    std::map<std::string,std::string> OtherOptions = std::map<std::string,std::string>();

    bool IsLinkDest = false;
    void YamlInsert(YAML::Emitter &Emitter);

    virtual ~Element() = default;
protected:
    virtual void YamlInsertBody(YAML::Emitter &Emitter);
};

#endif //SPOONTOOL_ELEMENT_H