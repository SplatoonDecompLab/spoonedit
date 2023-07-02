#include "virintox/gcore/Mesh.h"
#include<virintox/gcore/Texture.h>
#include<virintox/gcore/Shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include<virintox/gcore/stb_image.h>
#include<iostream>

namespace Graphics {
    Texture::Texture(boost::filesystem::path PngFile,bool Normalized) {
        glGenTextures(1, &TexID);
        glBindTexture(GL_TEXTURE_2D, TexID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStoref(GL_UNPACK_ALIGNMENT,1);
        glPixelStoref(GL_PACK_ALIGNMENT,1);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(PngFile.string().c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            if(nrChannels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            if(nrChannels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        stbi_image_free(data);
        if(!Normalized)
            size = glm::vec2(width,height);
        else 
            size = glm::vec2(1.0,1.0);

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    Texture::~Texture() {

    }

    void Texture::Draw(glm::vec2 pos, float scale) {
        static Mesh quadmesh = [](){
            glm::vec2 P1(0,0);
            glm::vec2 P2(1, 0);
            glm::vec2 P3(0,1);
            glm::vec2 P4(1,1);

            glm::vec2 TP1(0, 0);
            glm::vec2 TP2(1, 0);
            glm::vec2 TP3(0, 1);
            glm::vec2 TP4(1,1);

            return Mesh({P1,P2,P3,P2,P4,P3},{1,2,3,4,5,6},{TP1,TP2,TP3,TP2,TP4,TP3});
        }();

        quadmesh.Draw(*this, pos);
    }

    void Texture::DrawC(glm::vec2 pos, float scale) {
        Draw(pos - size * (scale / 2), scale);
    }

    void Texture::DrawTri(glm::tmat3x2<float> Tri, glm::tmat3x2<float> TexCoords) {

        GetDefaultShader2D().Use();
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::vector<float> vertices{
                Tri[0].x,Tri[0].y,TexCoords[0].x / size.x,TexCoords[0].y / size.y,
                Tri[1].x,Tri[1].y,TexCoords[1].x / size.x,TexCoords[1].y / size.y,
                Tri[2].x,Tri[2].y,TexCoords[2].x / size.x,TexCoords[2].y / size.y
        };
        unsigned int indices[] = {
                0,1,2
        };

        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);




        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Texture::SetActive() const{
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TexID);
    }
}