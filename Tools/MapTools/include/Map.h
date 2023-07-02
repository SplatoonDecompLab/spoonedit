//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_MAP_H
#define SPOONTOOL_MAP_H

#include<boost/filesystem.hpp>
#include<LevelObject.h>
#include<Rail.h>


struct Map {
    //STREEFUNCS
    Map();

    void Save(boost::filesystem::path FileLocation);

    boost::filesystem::path ActiveLocation = "";

    void Export(boost::filesystem::path YamlOut);

    std::vector<LevelObject> Objects;

    std::vector<Rail> Rails;


};

Map ConvertFromYaml(boost::filesystem::path File);


#endif //SPOONTOOL_MAP_H
