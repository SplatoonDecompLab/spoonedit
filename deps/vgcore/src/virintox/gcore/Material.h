//
// Created by tv on 18.07.23.
//
#include<assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Texture.h"

#ifndef SPOONTOOL_MATERIAL_H
#define SPOONTOOL_MATERIAL_H

namespace Graphics {

    class Material {
    public:
        explicit Material(aiMaterial* material, const boost::filesystem::path &path);

        Material(const Material&) = delete;

        Graphics::Texture m_texture;
        Graphics::Texture m_normalMap;
        Graphics::Texture m_emmissionMap;
        Graphics::Texture m_teamcolorMap;
        Graphics::Texture m_specularMap;

    };

} // Graphics

#endif //SPOONTOOL_MATERIAL_H
