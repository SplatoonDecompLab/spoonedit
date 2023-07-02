#include<Element.h>
#include<algorithm>

Element::Element(const YAML::Node &ObjNode,std::list<std::string> PropertyBlockList) {

    PropertyBlockList.emplace_back("Id");
    PropertyBlockList.emplace_back("UnitConfigName");
    PropertyBlockList.emplace_back("LayerConfigName");
    PropertyBlockList.emplace_back("Translate");
    PropertyBlockList.emplace_back("Rotate");
    PropertyBlockList.emplace_back("Scale");
    PropertyBlockList.emplace_back("Links");
    PropertyBlockList.emplace_back("ModelName");
    PropertyBlockList.emplace_back("IsLinkDest");


    runtimeID = static_cast<unsigned int>(rand());

    Name = ObjNode["Id"].as<std::string>();
    Type = ObjNode["UnitConfigName"].as<std::string>();

    TF.Position = ObjNode["Translate"];
    TF.Rotation = ObjNode["Rotate"];
    TF.Scale = ObjNode["Scale"];

    Layer = ToLayerConfig(ObjNode["LayerConfigName"].as<std::string>());

    ModelName = ObjNode["ModelName"].as<std::string>();
    IsLinkDest = ObjNode["IsLinkDest"].as<bool>();

    unsigned int ParamId = 1;

    while(YAML::Node ParamNode = ObjNode["Parameter" + std::to_string(ParamId)]){
        Parameters.push_back(ParamNode.as<long>());
        ParamId++;
    }

    unsigned int FloatParamId = 1;

    while(YAML::Node ParamNode = ObjNode["FloatParameter" + std::to_string(FloatParamId)]){
        FloatParameters.push_back(ParamNode.as<float>());
        FloatParamId++;
    }

    YAML::Node LinksNode = ObjNode["Links"];

    for(YAML::const_iterator it2 = LinksNode.begin();it2 != LinksNode.end();++it2) {
        auto node2 = it2->second;
        for(auto link: node2){
            Links.emplace_back(link);
        }
    }

    for(YAML::const_iterator it=ObjNode.begin();it != ObjNode.end();++it) {
        std::string OptName = it->first.as<std::string>();

        if(OptName.starts_with("Parameter") || OptName.starts_with("FloatParameter"))
            continue;

        if(std::any_of(PropertyBlockList.begin(), PropertyBlockList.end(),[&](std::string str){ return str == OptName;}))
            continue;



        auto node = it->second;

        auto str = node.as<std::string>();

        if(node.Tag() == "!l")
            str = "!l " + str;

        OtherOptions.insert({OptName,str});
    }


}

void Element::YamlInsert(YAML::Emitter &Emitter) {
    Emitter << YAML::BeginMap;
    YamlInsertBody(Emitter);
    Emitter << YAML::EndMap;
}

void Element::YamlInsertBody(YAML::Emitter &Emitter) {
    Emitter << YAML::Key << "Id" << YAML::Value << Name;

    TF.InsertIntoYaml(Emitter);

    Emitter << YAML::Key << "UnitConfigName" << YAML::Value << Type;

    Emitter << YAML::Key << "LayerConfigName" << YAML::Value << ToStringYaml(Layer);
    if(ModelName == "null") {
        Emitter << YAML::Key << "ModelName" << YAML::Value << YAML::LowerNull << YAML::Null;
    } else {
        Emitter << YAML::Key << "ModelName" << YAML::Value << ModelName;
    }

    Emitter << YAML::Key << "IsLinkDest" << YAML::Value << IsLinkDest;

    Emitter << YAML::Key << "Links" << YAML::BeginMap;

    std::map<std::string,std::vector<Link>> linksByType = std::map<std::string,std::vector<Link>>();

    for(Link link: Links){
        if(!linksByType.contains(link.Name))
            linksByType.insert({link.Name,std::vector<Link>()});
        linksByType.find(link.Name)->second.emplace_back(link);
    }


    for(auto links: linksByType){
        Emitter << YAML::Key << links.first;
        Emitter << YAML::BeginSeq;
        for(auto link: links.second){
            link.InsertIntoYaml(Emitter);
        }
        Emitter << YAML::EndSeq;
    }
    Emitter << YAML::EndMap;

    for(int i = 1; i <= Parameters.size(); i++){
        Emitter << YAML::Key << "Parameter" + std::to_string(i) << YAML::Value << YAML::LocalTag("l") << Parameters[i-1];
    }

    for(int i = 1; i <= FloatParameters.size(); i++){
        Emitter << YAML::Key << "FloatParameter" + std::to_string(i) << YAML::Value << FloatParameters[i-1];
    }

    for(auto opts: OtherOptions){
        Emitter << YAML::Key << opts.first;
        if(opts.second == "null") {
            Emitter << YAML::Value << YAML::CamelNull << YAML::Null;
        }
        else {
            if(boost::starts_with(opts.second,"!l ")){
                auto str = opts.second.substr(3);

                Emitter << YAML::Value << YAML::LocalTag("l") << std::stol(str);
            } else
                Emitter << YAML::Value << opts.second;
        }
    }
}
