//
// Created by tv on 18.07.23.
//

#include "Material.h"

namespace Graphics {
    Material::Material(aiMaterial *mat, const boost::filesystem::path &path):
    m_texture([&](){
        if(mat->GetTextureCount(aiTextureType_DIFFUSE) >= 1){
            aiString string;
            mat->GetTexture(aiTextureType_DIFFUSE, 0, &string);
            return path / string.C_Str();
        } else {
            return boost::filesystem::current_path() / "Models" / "Missing.png";
        }
    }()),
    m_normalMap([&](){
        if(mat->GetTextureCount(aiTextureType_NORMALS) >= 1){
            aiString string;
            mat->GetTexture(aiTextureType_NORMALS, 0, &string);
            return path / string.C_Str();
        } else {
            return boost::filesystem::current_path() / "Models" / "DefaultNormal.png";
        }
    }()),
    m_emmissionMap([&](){
        if(mat->GetTextureCount(aiTextureType_EMISSIVE) >= 1){
            aiString string;
            mat->GetTexture(aiTextureType_EMISSIVE, 0, &string);
            return path / string.C_Str();
        } else {
            return boost::filesystem::current_path() / "Models" / "DefaultEmmission.png";
        }
    }()),
    m_specularMap([&](){
        if(mat->GetTextureCount(aiTextureType_SPECULAR) >= 1){
            aiString string;
            mat->GetTexture(aiTextureType_SPECULAR, 0, &string);
            return path / string.C_Str();
        } else {
            return boost::filesystem::current_path() / "Models" / "DefaultSpecular.png";
        }
    }()),
    m_teamcolorMap([&](){
        if(boost::filesystem::exists(path / (std::string(mat->GetName().C_Str()) + "_Tcl.png"))){
            boost::filesystem::path texPath = path / (std::string(mat->GetName().C_Str()) + "_Tcl.png");
            return texPath;
        } else {

            return boost::filesystem::current_path() / "Models" / "DefaultTeamcolor.png";
        }
    }()) {}
} // Graphics