//
// Created by tv on 21.05.23.
//

#include "Model.h"
#include "assimp/scene.h"
#include "MainViewport.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <virintox/gcore/Graphics.h>
#include "Transform.h"

Model::Model(std::string modelname) {
    auto modeltoload = boost::filesystem::current_path() / "Models" / modelname / (modelname + ".dae");

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( modeltoload.string(),
                                              aiProcess_CalcTangentSpace       |
                                              aiProcess_Triangulate            |
                                              aiProcess_JoinIdenticalVertices  |
//                                              aiProcess_Debone                 |
                                              aiProcess_FlipUVs                |
                                              aiProcess_SortByPType);

    if(!scene)
    {
        Graphics::window->msgBoxes.push_back(new Graphics::MessageBox(importer.GetErrorString(),Graphics::ErrorSeverity::Error));
        return;
    }

    for(unsigned int i = 0; i < scene->mNumMeshes; i++){
        auto mesh = scene->mMeshes[i];
        auto mat = scene->mMaterials[mesh->mMaterialIndex];

        std::string firsttex;

        for(unsigned int currenttex = 0; currenttex < mat->GetTextureCount(aiTextureType_DIFFUSE); currenttex++){
            aiString string;
            mat->GetTexture(aiTextureType_DIFFUSE, 0, &string);

            if(firsttex == "")
                firsttex = string.C_Str();
            else
                Graphics::window->msgBoxes.push_back(new Graphics::MessageBox("SUS ඞ",Graphics::ErrorSeverity::Error));

            Textures.emplace_back((boost::filesystem::current_path() / "Models" / modelname / string.C_Str()));
        }

        if(firsttex == ""){
            Textures.emplace_back((boost::filesystem::current_path() / "Models" / "Missing.png"));
        }

        auto vertices = std::vector<glm::vec3>();
        auto texcoords = std::vector<glm::vec2>();
        auto indices = std::vector<GLuint>();

        for(unsigned int verticienum = 0; verticienum < mesh->mNumVertices; verticienum++){

            vertices.emplace_back(mesh->mVertices[verticienum].x,mesh->mVertices[verticienum].y,mesh->mVertices[verticienum].z);

            if (mesh->HasTextureCoords(0)) // Only slot [0] is in question.
            {
                texcoords.emplace_back(mesh->mTextureCoords[0][verticienum].x,mesh->mTextureCoords[0][verticienum].y);
            }
            else
                texcoords.emplace_back(0.0f, 0.0f);
        }

        for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3) {
            assert(mesh->mFaces[i3].mNumIndices == 3 && "Faces must always have 3 indices because other ammounts of indices are not supported by opengl!");

            for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
                indices.push_back(mesh->mFaces[i3].mIndices[i4]);
        }

        Meshes.emplace_back(vertices,indices,texcoords,GetMainViewport()->shader);
    }
}

void Model::Draw(Transform tf) {

    unsigned int texnum = 0;
    for(auto mesh: Meshes){

        glm::vec3 pos(tf.Position.X,tf.Position.Y,tf.Position.Z);
        glm::vec3 rot(-tf.Rotation.X,tf.Rotation.Y,-tf.Rotation.Z);
        glm::vec3 scale(tf.Scale.X,tf.Scale.Y,tf.Scale.Z);

        mesh.Draw(Textures[texnum],pos,scale,rot);
        texnum++;
    }

}

void Model::DrawSelection(Transform tf) {
    unsigned int texnum = 0;

    static Graphics::Texture selectionTex = Graphics::Texture((boost::filesystem::current_path() / "Models" / "selected.png"));
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    for(auto mesh: Meshes){

        glm::vec3 pos(tf.Position.X,tf.Position.Y,tf.Position.Z);
        glm::vec3 rot(-tf.Rotation.X,tf.Rotation.Y,tf.Rotation.Z);
        glm::vec3 scale(tf.Scale.X,tf.Scale.Y,tf.Scale.Z);



        mesh.Draw(selectionTex,pos,scale,rot);
        texnum++;
    }


    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

}


