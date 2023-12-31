//
// Created by tv on 21.05.23.
//

#ifndef SPOONTOOL_MODEL_H
#define SPOONTOOL_MODEL_H


#include <boost/filesystem/path.hpp>
#include<map>
#include "virintox/gcore/Mesh.h"
#include "Transform.h"
#include "virintox/gcore/Material.h"

class Model {
public:
    Model(std::string modelName);

    Model(const Model& mdl) = delete;

    Model(Model&&) = default;

    void Draw(const Transform &tf, Graphics::Shader &shader, const glm::mat4 &VP);

    void DrawSelection(Transform tf, Graphics::Shader &shader, glm::mat4 VP);
protected:
    std::list<std::unique_ptr<Graphics::Mesh>> Meshes;
    std::vector<std::unique_ptr<Graphics::Material>> Materials;

    //std::map<std::string,Graphics::Texture> TexturesForName;
};

#endif //SPOONTOOL_MODEL_H
