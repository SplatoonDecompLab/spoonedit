#pragma once

#include "glm/ext/vector_float3.hpp"
#include"glm/mat3x2.hpp"
#include"glm/mat3x3.hpp"
#include"glm/vec3.hpp"
#include"glm/vec2.hpp"
#include<vector>

#include"Texture.h"
#include<GL/glew.h>
#include"Shader.h"
#include "Material.h"

namespace Graphics {
    class Mesh {
    public:

        //explicit Mesh(boost::filesystem::path PathToModel,Shader &ShaderToUse);

        //explicit Mesh(const std::vector<glm::mat3x2> &Polygons, const std::vector<glm::mat3x2> &TexCoords,const std::vector<glm::vec3> &Normals, Shader &Shader = GetDefaultShader2D());
        Mesh(const std::vector<glm::vec2> &Points, const std::vector<GLuint> &Indices,
             const std::vector<glm::vec2> &TexCoords, const std::vector<glm::vec3> &Normals);

        //explicit Mesh(const std::vector<glm::mat3x3> &Polygons, const std::vector<glm::mat3x2> &TexCoords,const std::vector<glm::vec3> &Normals, Shader &Shader = GetDefaultShader3D());
        Mesh(const std::vector<glm::vec3> &Points, const std::vector<GLuint> &Indices,
             const std::vector<glm::vec2> &TexCoords, const std::vector<glm::vec3> &Normals,
             const std::vector<glm::vec3> &Tangents,const std::vector<glm::vec3> &Bitangents);

        //void Draw(const Texture &Tex, const glm::vec2 &Pos) const;

        Mesh(const Mesh&) = delete;

        Mesh(Mesh&&) = default;

        void Draw(const Graphics::Material &mat) const;

        ~Mesh();
    private:
        GLuint VBO, VAO, EBO;
        GLuint VCount;

    };
}