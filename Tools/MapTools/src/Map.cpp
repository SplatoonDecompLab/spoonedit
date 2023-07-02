//
// Created by tv on 30.04.23.
//

#include<Map.h>
#include<yaml-cpp/yaml.h>
#include<boost/iostreams/filtering_stream.hpp>
//#include<boost/iostreams/filter/lzma.hpp>
#include <boost/iostreams/device/file.hpp>
#include<iostream>
#include<boost/algorithm/string.hpp>

//GENSERIALIZABLECONTENT(Map,(Objects,Rails))

Map::Map(): Objects(), Rails() {

}

void Map::Save(boost::filesystem::path FileLocation) {

}

void Map::Export(boost::filesystem::path YamlOut) {
    YAML::Emitter Emitter;

    Emitter << YAML::BeginMap;
    Emitter << YAML::Key << "Version" << YAML::Value << "1";
    Emitter << YAML::Key << "IsBigEndian" << YAML::Value << "True";
    Emitter << YAML::Key << "SupportPaths" << YAML::Value << "False";
    Emitter << YAML::Key << "HasReferenceNodes" << YAML::Value << "False";


    Emitter << YAML::Key << "root" << YAML::BeginMap;
    Emitter << YAML::Key << "FilePath" << YAML::Value << "NoFilePathInsertMegamindMemeHere";
    Emitter << YAML::Key << "Objs" << YAML::BeginSeq;

    for(auto& obj: Objects){
        obj.YamlInsert(Emitter);
    }
    //Objs
    Emitter << YAML::EndSeq;

    Emitter << YAML::Key << "Rails" << YAML::BeginSeq;

    for(auto obj: Rails){
        obj.YamlInsert(Emitter);
    }
    Emitter << YAML::EndSeq;

    //root
    Emitter << YAML::EndMap;
    //main document
    Emitter << YAML::EndMap;

    std::ofstream file(YamlOut.string());

    std::string str = Emitter.c_str();

    boost::algorithm::replace_all(str,"~","null");

    file << str;
    file.flush();
    file.close();
}



Map ConvertFromYaml(boost::filesystem::path File){
    Map OutputMap = Map();

    YAML::Node MapYaml = YAML::LoadFile(File.generic_string());
    auto RootNode = MapYaml["root"];

    auto ObjList = RootNode["Objs"];

    if(!ObjList.IsSequence()) throw std::runtime_error("Something went Wrong!");


    for (YAML::iterator it = ObjList.begin(); it != ObjList.end(); it++) {
        const YAML::Node& obj = *it;

        OutputMap.Objects.push_back(LevelObject(obj));
    }

    auto RailList = RootNode["Rails"];

    for (YAML::iterator it = RailList.begin(); it != RailList.end(); it++) {
        const YAML::Node& obj = *it;

        Rail newObj(obj);

        OutputMap.Rails.push_back(newObj);
    }



    std::cout << "Successfully converted to smap!" << std::endl;

    return OutputMap;
}
