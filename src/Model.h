//
// Created by tv on 21.05.23.
//

#ifndef SPOONTOOL_MODEL_H
#define SPOONTOOL_MODEL_H


#include <boost/filesystem/path.hpp>
#include<map>
#include "virintox/gcore/Mesh.h"
#include "Transform.h"

class Model {
public:
    Model(std::string modelName);

    void Draw(Transform tf);

    void DrawSelection(Transform tf);
protected:
    std::vector<Graphics::Mesh> Meshes;
    std::vector<Graphics::Texture> Textures;

    //std::map<std::string,Graphics::Texture> TexturesForName;
};

#endif //SPOONTOOL_MODEL_H
