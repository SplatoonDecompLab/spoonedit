#include<Rail.h>



Rail::Rail(const YAML::Node &ObjNode) : Element(ObjNode, {"RailPoints","RailType","IsClosed","IsLadder","Priority"}) {
    YAML::Node railPointsNode = ObjNode["RailPoints"];

    for (auto rlpoint: railPointsNode) {
        Points.emplace_back(rlpoint);
    }

    m_railType = ToRailType(ObjNode["RailType"].as<std::string>());
    IsClosed = ObjNode["IsClosed"].as<bool>();
    IsLadder = ObjNode["IsLadder"].as<bool>();
    Priority = ObjNode["Priority"].as<long>();
}

Rail::Rail(const Vector3 &Position) {
    TF.Position = Position;
    Name = "Unnamed Rail";
    Type = "Rail";
    Points.emplace_back(Position);
}

void Rail::YamlInsertBody(YAML::Emitter &Emitter,bool compiled) {
    Element::YamlInsertBody(Emitter);

    Emitter << YAML::Key << "RailPoints" << YAML::BeginSeq;
    for (auto point: Points) {
        point.YamlInsert(Emitter);
    }
    Emitter << YAML::EndSeq;

    Emitter << YAML::Key << "RailType" << YAML::Key << ToString(m_railType);
    Emitter << YAML::Key << "IsClosed" << YAML::Key << IsClosed;
    Emitter << YAML::Key << "IsLadder" << YAML::Key << IsLadder;
    Emitter << YAML::Key << "Priority" << YAML::Key << YAML::LocalTag("l") << Priority;
}
