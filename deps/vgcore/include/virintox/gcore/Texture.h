#ifndef PNS_TEXTURE_H
#define PNS_TEXTURE_H
#include<GL/glew.h>
#include<boost/filesystem.hpp>
#include<glm/mat3x2.hpp>
#include<glm/vec2.hpp>
namespace Graphics {
    class Texture {
    public:
        Texture() = default;

        Texture(boost::filesystem::path PngFile,bool Normalize = false);

        ~Texture();

        glm::vec2 size;

        inline operator int() const{
            return TexID;
        }
        void SetActive() const;
        void Draw(glm::vec2 pos, float scale = 1);
        void DrawC(glm::vec2 pos, float scale = 1);
        void DrawTri(glm::tmat3x2<float> Tri, glm::tmat3x2<float> TexCoords);

    protected:
        unsigned int TexID;
    };
}
#endif //PNS_TEXTURE_H
