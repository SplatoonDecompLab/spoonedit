#include<LevelObject.h>
#include<SpecialObjectParamVersions/All.h>

LevelObject::LevelObject(const YAML::Node &ObjNode): Element(ObjNode,{"Team","DropId"}) {
    Team = ObjNode["Team"].as<long>(-1);
    DropId = ObjNode["DropId"].as<long>(DropId::Default);
}

void LevelObject::YamlInsertBody(YAML::Emitter &Emitter) {
    Element::YamlInsertBody(Emitter);
    Emitter << YAML::Key << "Team" << YAML::Value << YAML::LocalTag("l") << Team;
    Emitter << YAML::Key << "DropId" << YAML::Value << YAML::LocalTag("l") << DropId;
}