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
#include<glm/mat4x4.hpp>
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/matrix_transform.hpp"
#include<glm/mat3x2.hpp>
#include<glm/mat3x3.hpp>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>

Model::Model(std::string modelname)  {
    CopyCount = new unsigned(1);

    auto modeltoload = boost::filesystem::current_path() / "Models" / modelname / (modelname + ".dae");

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile( modeltoload.string(),
                                              aiProcess_CalcTangentSpace       |
                                              aiProcess_Triangulate            |
                                              aiProcess_JoinIdenticalVertices  |
//                                              aiProcess_Debone                 |
                                              aiProcess_GenNormals             |
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

        Materials.push_back(new Graphics::Material(mat,boost::filesystem::current_path() / "Models" / modelname));

        auto vertices = std::vector<glm::vec3>();
        auto texcoords = std::vector<glm::vec2>();
        auto normals = std::vector<glm::vec3>();
        auto tangents = std::vector<glm::vec3>();
        auto bitangents = std::vector<glm::vec3>();
        auto indices = std::vector<GLuint>();

        for(unsigned int verticienum = 0; verticienum < mesh->mNumVertices; verticienum++){

            vertices.emplace_back(mesh->mVertices[verticienum].x,mesh->mVertices[verticienum].y,mesh->mVertices[verticienum].z);
            normals.emplace_back(mesh->mNormals[verticienum].x,mesh->mNormals[verticienum].y,mesh->mNormals[verticienum].z);



            if (mesh->HasTextureCoords(0)) // Only slot [0] is in question.
            {
                texcoords.emplace_back(mesh->mTextureCoords[0][verticienum].x,mesh->mTextureCoords[0][verticienum].y);
                tangents.emplace_back(mesh->mTangents[verticienum].x,mesh->mTangents[verticienum].y,mesh->mTangents[verticienum].z);
                bitangents.emplace_back(mesh->mBitangents[verticienum].x,mesh->mBitangents[verticienum].y,mesh->mBitangents[verticienum].z);
            }
            else {
                texcoords.emplace_back(0.0f, 0.0f);
                tangents.emplace_back(mesh->mNormals[verticienum].x,mesh->mNormals[verticienum].y,mesh->mNormals[verticienum].z);
                bitangents.emplace_back(mesh->mNormals[verticienum].x,mesh->mNormals[verticienum].y,mesh->mNormals[verticienum].z);
            }
        }

        for (unsigned int i3 = 0; i3 < mesh->mNumFaces; ++i3) {
            assert(mesh->mFaces[i3].mNumIndices == 3 && "Faces must always have 3 indices because other ammounts of indices are not supported by opengl!");

            for (unsigned int i4 = 0; i4 < mesh->mFaces[i3].mNumIndices; ++i4)
                indices.push_back(mesh->mFaces[i3].mIndices[i4]);
        }

        Meshes.push_back(new Graphics::Mesh(vertices,indices,texcoords,normals,tangents,bitangents));
    }
}

void Model::Draw(const Transform &tf, Graphics::Shader &shader, const glm::mat4 &VP) {

    unsigned int texnum = 0;
    for(auto& mesh: Meshes){

        glm::vec3 pos(tf.Position.X,tf.Position.Y,tf.Position.Z);
        glm::vec3 rot(tf.Rotation.X,tf.Rotation.Y,tf.Rotation.Z);
        glm::vec3 normalRot(tf.Rotation.X,tf.Rotation.Y,tf.Rotation.Z);
        glm::vec3 scale(tf.Scale.X,tf.Scale.Y,tf.Scale.Z);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1),glm::vec3(pos.x,pos.y,pos.z));
        glm::mat4 translationNoYInvertMatrix = glm::translate(glm::mat4(1),glm::vec3(pos.x,pos.y,pos.z));
        glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1),glm::radians(rot.z), glm::vec3(0,0,1));
        glm::mat4 rotationMatrixY = glm::rotate(rotationMatrixZ,glm::radians(rot.y), glm::vec3(0,1,0));
        glm::mat4 rotationMatrix = glm::rotate(rotationMatrixY,glm::radians(rot.x), glm::vec3(1,0,0));

        glm::mat4 nrmRotationMatrixZ = glm::rotate(glm::mat4(1),glm::radians(normalRot.z), glm::vec3(0,0,1));
        glm::mat4 nrmRotationMatrixY = glm::rotate(nrmRotationMatrixZ,glm::radians(normalRot.y), glm::vec3(0,1,0));
        glm::mat4 nrmRotationMatrix = glm::rotate(nrmRotationMatrixY,glm::radians(normalRot.x), glm::vec3(1,0,0));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1),scale);

        glm::mat4 matrix = VP * translationMatrix * rotationMatrix * scaleMatrix;
        glm::mat4 ModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        glm::mat4 noYInvertMatrix = translationNoYInvertMatrix * nrmRotationMatrix * scaleMatrix;

        GLuint MatrixID = shader.getUniformLocation("MVP");
        glUniformMatrix4fv(MatrixID,1,GL_FALSE,&matrix[0][0]);

        GLuint ModelMatrixID = shader.getUniformLocation("ModelMatrix");
        glUniformMatrix4fv(ModelMatrixID,1,GL_FALSE,&ModelMatrix[0][0]);

        GLuint MatrixNoYInvID = shader.getUniformLocation("TransformationMatrixNoYInv");
        glUniformMatrix4fv(MatrixNoYInvID,1,GL_FALSE,&noYInvertMatrix[0][0]);

        GLuint RotationMatrixID = shader.getUniformLocation("NormalRotationMatrix");
        glUniformMatrix4fv(RotationMatrixID,1,GL_FALSE,&nrmRotationMatrix[0][0]);

        mesh->Draw(Materials[texnum]);
        texnum++;
    }

}

void Model::DrawSelection(Transform tf, Graphics::Shader &shader, glm::mat4 VP) {
    unsigned int texnum = 0;

    static Graphics::Texture selectionTex = Graphics::Texture((boost::filesystem::current_path() / "Models" / "selected.png"));
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    for(auto &mesh: Meshes){

        glm::vec3 pos(tf.Position.X,tf.Position.Y,tf.Position.Z);
        glm::vec3 rot(-tf.Rotation.X,tf.Rotation.Y,tf.Rotation.Z);
        glm::vec3 scale(tf.Scale.X,tf.Scale.Y,tf.Scale.Z);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1),glm::vec3(pos.x,-pos.y,pos.z));
        glm::mat4 translationNoYInvertMatrix = glm::translate(glm::mat4(1),glm::vec3(pos.x,pos.y,pos.z));
        glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1),glm::radians(rot.z), glm::vec3(0,0,1));
        glm::mat4 rotationMatrixY = glm::rotate(rotationMatrixZ,glm::radians(rot.y), glm::vec3(0,1,0));
        glm::mat4 rotationMatrix = glm::rotate(rotationMatrixY,glm::radians(rot.x), glm::vec3(1,0,0));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1),scale);

        glm::mat4 matrix = VP * translationMatrix * rotationMatrix * scaleMatrix;
        glm::mat4 noYInvertMatrix = translationNoYInvertMatrix * rotationMatrix * scaleMatrix;

        GLuint MatrixID = shader.getUniformLocation("MVP");
        glUniformMatrix4fv(MatrixID,1,GL_FALSE,&matrix[0][0]);

        GLuint MatrixNoYInvID = shader.getUniformLocation("TransformationMatrixNoYInv");
        glUniformMatrix4fv(MatrixNoYInvID,1,GL_FALSE,&noYInvertMatrix[0][0]);



        mesh->Draw(Materials[texnum]);
        texnum++;
    }


    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

}

Model::~Model() {
    (*CopyCount)--;

    if((*CopyCount) == 0) {
        for (Graphics::Material *mat: Materials)
            delete mat;

        for (Graphics::Mesh *mesh: Meshes)
            delete mesh;
    }
}

Model::Model(const Model &mdl): CopyCount(mdl.CopyCount), Meshes(mdl.Meshes), Materials(mdl.Materials) {
    (*CopyCount)++;
}


