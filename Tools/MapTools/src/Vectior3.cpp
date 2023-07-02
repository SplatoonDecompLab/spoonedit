#include<Vector3.h>

Vector3::Vector3(YAML::Node node) {
    try {
        X = node["X"].as<float>();
        Y = node["Y"].as<float>();
        Z = node["Z"].as<float>();
    } catch(...){
        std::cout << "WER AUCH IMMER HIER SEINEN PC AUF DEUTSCH GESTELLT HAT\n 1. STELL ES UM DAMIT DU KEINE PROBLEME MEHR HAST\n 2. TF\n 3. KEINE KOMPATIBILITÄT GARANTIERT UND NUR FÜR DICH MUSSTE ICH EINEN PATCH SCHREIBEN. \nFFS >:(.\n";

        auto xstr = node["X"].as<std::string>();
        boost::algorithm::replace_all(xstr,",",".");
        X = std::stof(xstr);

        auto ystr = node["Y"].as<std::string>();
        boost::algorithm::replace_all(ystr,",",".");
        Y = std::stof(ystr);

        auto zstr = node["Z"].as<std::string>();
        boost::algorithm::replace_all(zstr,",",".");
        Z = std::stof(zstr);
    }
}