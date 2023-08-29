#include"Mesh.h"
#include"glm/mat4x4.hpp"

#include"assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "glm/ext/matrix_transform.hpp"
#include "assimp/scene.h"
#include "Graphics.h"


namespace Graphics{

    //Mesh::Mesh(boost::filesystem::path PathToModel, Shader &ShaderToUse): shader(ShaderToUse){

    //}

    Mesh::Mesh(const std::vector<glm::vec2> &Points,const std::vector<GLuint> &Indices, const std::vector<glm::vec2> &TexCoords,const std::vector<glm::vec3> &Normals){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
 /*       glCullFace(GL_BACK);*/

        VCount = Indices.size();
        
        std::vector<float> VertexData{};
        for(int x = 0; x < Points.size() || x < TexCoords.size(); x++){
            VertexData.push_back(Points[x].x);
            VertexData.push_back(Points[x].y);

            VertexData.push_back(TexCoords[x].x);
            VertexData.push_back(TexCoords[x].y);

            VertexData.push_back(Normals[x].x);
            VertexData.push_back(Normals[x].y);
            VertexData.push_back(Normals[x].z);

        }
        

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), Indices.data(), GL_STATIC_DRAW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    Mesh::Mesh(const std::vector<glm::vec3> &Points, const std::vector<GLuint> &Indices,
               const std::vector<glm::vec2> &TexCoords, const std::vector<glm::vec3> &Normals,
               const std::vector<glm::vec3> &Tangents,const std::vector<glm::vec3> &Bitangents) {
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

            VertexData.push_back(Normals[x].x);
            VertexData.push_back(Normals[x].y);
            VertexData.push_back(Normals[x].z);

            VertexData.push_back(Tangents[x].x);
            VertexData.push_back(Tangents[x].y);
            VertexData.push_back(Tangents[x].z);

            VertexData.push_back(Bitangents[x].x);
            VertexData.push_back(Bitangents[x].y);
            VertexData.push_back(Bitangents[x].z);
        }


        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
        glEnableVertexAttribArray(4);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Mesh::Draw(const Graphics::Material &mat) const{
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        mat.m_texture.setActive();
        mat.m_normalMap.setActive(1);
        mat.m_specularMap.setActive(2);
        mat.m_emmissionMap.setActive(3);
        mat.m_teamcolorMap.setActive(4);

/*        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);*/

        const GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, buffers);

        glDrawElements(
                GL_TRIANGLES,      // mode
                VCount,    // count
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
        );
    }

    /*void Mesh::Draw(const Texture &Tex,const glm::vec2 &Pos) const{
        glm::mat4x4 matrix{{1,0,0,Pos.x},
                           {0,1,0,Pos.y},
                           {0,0,1,0},
                           {0,0,0,1}};

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        Tex.setActive();

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
    }*/

    Mesh::~Mesh() {
        glDeleteVertexArrays(1,&VAO);
        glDeleteBuffers(1,&VBO);
        glDeleteBuffers(1,&EBO);
    }
}