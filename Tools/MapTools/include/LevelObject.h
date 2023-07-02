//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_LEVELOBJECT_H
#define SPOONTOOL_LEVELOBJECT_H

#include<Transform.h>
#include<Link.h>

#include<yaml-cpp/yaml.h>
#include "SpecialObjectParams.h"
#include "Element.h"


namespace Teams{
    constexpr long Player = 0;
    constexpr long Neutral = 1;
    constexpr long Enemy = 2;

    constexpr std::array<long,3> AllOptions = {Player,Neutral,Enemy};

    inline constexpr std::string TeamToText(long team) {
        switch(team){
            case 0:
                return "Player";
            case 1:
                return "Neutral";
            case 2:
                return "Enemy";
            default:
                return "Invalid";
        }
    }
}

namespace DropId{
    constexpr long Default = -1;
    constexpr long Armor = 8;
    constexpr long Map = 9;
    constexpr long Key = 10;
    constexpr long OneSphere = 11;
    constexpr long FiveSphere = 12;
    constexpr long TenSphere = 13;
    constexpr long Bubbler = 14;
    constexpr long Bazooka = 15;

    constexpr std::array<long,9> AllOptions = {Default,Armor,Map,Key,OneSphere,FiveSphere,TenSphere,Bubbler,Bazooka};

    inline constexpr std::string DropIdToText(long team) {
        switch(team){
            case -1:
                return "Default";
            case 8:
                return "Armor";
            case 9:
                return "Map";
            case 10:
                return "Key";
            case 11:
                return "1 Sphere";
            case 12:
                return "5 Spheres";
            case 13:
                return "10 Spheres";
            case 14:
                return "Bubbler";
            case 15:
                return "Bazooka";
            default:
                return "Invalid";
        }
    }
}

struct LevelObject: public Element{
    LevelObject(const YAML::Node &ObjNode);

protected:
    void YamlInsertBody(YAML::Emitter &Emitter) override;
public:
    long Team;
    long DropId;

    ~LevelObject() = default;
};


#endif //SPOONTOOL_LEVELOBJECT_H
