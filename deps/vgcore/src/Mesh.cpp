#include<virintox/gcore/Mesh.h>
#include<glm/mat4x4.hpp>

#include<assimp/Importer.hpp>
#include "assimp/postprocess.h"
#include "glm/ext/matrix_transform.hpp"
#include <assimp/scene.h>
#include <virintox/gcore/Graphics.h>

namespace Graphics{

    //Mesh::Mesh(boost::filesystem::path PathToModel, Shader &ShaderToUse): shader(ShaderToUse){

    //}

    Mesh::Mesh(const std::vector<glm::vec2> &Points,const std::vector<GLuint> &Indices, const std::vector<glm::vec2> &TexCoords, Shader &ShaderToUse) : shader(ShaderToUse){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glCullFace(GL_BACK);

        VCount = Indices.size();
        
        std::vector<float> VertexData{};
        for(int x = 0; x < Points.size() || x < TexCoords.size(); x++){
            VertexData.push_back(Points[x].x);
            VertexData.push_back(Points[x].y);
            VertexData.push_back(TexCoords[x].x);
            VertexData.push_back(TexCoords[x].y);
        }
        

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), Indices.data(), GL_STATIC_DRAW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Mesh::Mesh(const std::vector<glm::vec3> &Points, const std::vector<GLuint> &Indices,
               const std::vector<glm::vec2> &TexCoords, Shader &Shader) : shader(Shader) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glCullFace(GL_BACK);

        VCount = Indices.size();

        assert(Indices.size() % 3 == 0 && "Indices must always come in pairs of three since a face consists of 3 points!");

        std::vector<float> VertexData{};
        for(int x = 0; x < Points.size() || x < TexCoords.size(); x++){
            VertexData.push_back(Points[x].x);
            VertexData.push_back(Points[x].y);
            VertexData.push_back(Points[x].z);

            VertexData.push_back(TexCoords[x].x);
            VertexData.push_back(TexCoords[x].y);
        }


        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Mesh::Draw(const Texture &Tex,const glm::vec3 &Pos, const glm::vec3 &Scale, const glm::vec3 &Rot) const{
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1),glm::vec3(Pos.x,-Pos.y,Pos.z));
        glm::mat4 translationNoYInvertMatrix = glm::translate(glm::mat4(1),glm::vec3(Pos.x,Pos.y,Pos.z));
        glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1),glm::radians(Rot.z), glm::vec3(0,0,1));
        glm::mat4 rotationMatrixY = glm::rotate(rotationMatrixZ,glm::radians(Rot.y), glm::vec3(0,1,0));
        glm::mat4 rotationMatrix = glm::rotate(rotationMatrixY,glm::radians(Rot.x), glm::vec3(1,0,0));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1),Scale);

        glm::mat4 matrix = translationMatrix * rotationMatrix * scaleMatrix;
        glm::mat4 noYInvertMatrix = translationNoYInvertMatrix * rotationMatrix * scaleMatrix;

        GLuint MatrixID = glGetUniformLocation(shader.ShaderId, "TransformationMatrix");
        glUniformMatrix4fv(MatrixID,1,GL_FALSE,&matrix[0][0]);

        GLuint MatrixNoYInvID = glGetUniformLocation(shader.ShaderId, "TransformationMatrixNoYInv");
        glUniformMatrix4fv(MatrixNoYInvID,1,GL_FALSE,&noYInvertMatrix[0][0]);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        shader.Use();
        Tex.SetActive();

        const GLenum buffers[] = {GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(4, buffers);


        glDrawElements(
                GL_TRIANGLES,      // mode
                VCount,    // count
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
        );
    }

    void Mesh::Draw(const Texture &Tex,const glm::vec2 &Pos) const{
        glm::mat4x4 matrix{{1,0,0,Pos.x},
                           {0,1,0,Pos.y},
                           {0,0,1,0},
                           {0,0,0,1}};
        GLuint MatrixID = glGetUniformLocation(shader.ShaderId, "TransformationMatrix");
        glUniformMatrix4fv(MatrixID,1,GL_FALSE,&matrix[0][0]);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        shader.Use();
        Tex.SetActive();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        const GLenum buffers[] = {GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(4, buffers);


        glDrawElements(
                GL_TRIANGLES,      // mode
                VCount,    // count
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
        );
    }
}