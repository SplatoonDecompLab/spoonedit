#include<LevelObject.h>
#include<SpecialObjectParamVersions/All.h>

LevelObject::LevelObject(const YAML::Node &ObjNode): Element(ObjNode,{"Team","DropId","IsStatic"}) {
    Team = ObjNode["Team"].as<long>(-1);
    DropId = ObjNode["DropId"].as<long>(DropId::Default);
    m_isStatic = ObjNode["IsStatic"].as<bool>(false);

}

void LevelObject::YamlInsertBody(YAML::Emitter &Emitter,bool compiled) {
    Element::YamlInsertBody(Emitter);
    Emitter << YAML::Key << "Team" << YAML::Value << YAML::LocalTag("l") << Team;
    Emitter << YAML::Key << "DropId" << YAML::Value << YAML::LocalTag("l") << DropId;
    if(!compiled){
        Emitter << YAML::Key << "IsStatic" << YAML::Value << m_isStatic;
    }
}