#pragma once
#include "glm/ext/vector_float3.hpp"
#include<glm/mat3x2.hpp>
#include<glm/mat3x3.hpp>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
#include<vector>

#include<virintox/gcore/Texture.h>
#include<GL/glew.h>
#include<virintox/gcore/Shader.h>

namespace Graphics{
    class Mesh{
    public:

        explicit Mesh(boost::filesystem::path PathToModel,Shader &ShaderToUse);

        explicit Mesh(const std::vector<glm::mat3x2> &Polygons, const std::vector<glm::mat3x2> &TexCoords, Shader &Shader = GetDefaultShader2D());
        Mesh(const std::vector<glm::vec2> &Points,const std::vector<GLuint> &Indices,const std::vector<glm::vec2> &TexCoords, Shader &Shader = GetDefaultShader2D());
        explicit Mesh(const std::vector<glm::mat3x3> &Polygons, const std::vector<glm::mat3x2> &TexCoords, Shader &Shader = GetDefaultShader3D());
        Mesh(const std::vector<glm::vec3> &Points,const std::vector<GLuint> &Indices, const std::vector<glm::vec2> &TexCoords, Shader &Shader = GetDefaultShader3D());

        void Draw(const Texture &Tex,const glm::vec2 &Pos) const;
        void Draw(const Texture &Tex,const glm::vec3 &Pos, const glm::vec3 &Scale = glm::vec3(1,1,1), const glm::vec3 &Rot = glm::vec3(0,0,0)) const;
    private:
        Shader &shader;
        GLuint VBO,VAO,EBO;
        GLuint VCount;

    };
}