//
// Created by tv on 30.04.23.
//

#ifndef SPOONTOOL_MAP_H
#define SPOONTOOL_MAP_H

#include<boost/filesystem.hpp>
#include<LevelObject.h>
#include<Rail.h>
#include<filesystem>

struct FieldOptions{
    std::string m_fieldName = "Fld_Unk";

};

struct Map {
    Map();
    //Loads file from custom map format
    Map(boost::filesystem::path file);

    void Save(boost::filesystem::path FileLocation);

    boost::filesystem::path ActiveLocation = "";

    void Export(boost::filesystem::path YamlOut);

    std::list<LevelObject> Objects;

    std::list<Rail> Rails;

    std::filesystem::path m_filePath;

    FieldOptions m_fieldOptions;

    Element* GetElementById(std::string id);
};

Map ConvertFromYaml(boost::filesystem::path File);



#endif //SPOONTOOL_MAP_H
