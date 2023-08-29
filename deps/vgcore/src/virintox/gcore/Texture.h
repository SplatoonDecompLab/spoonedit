#ifndef PNS_TEXTURE_H
#define PNS_TEXTURE_H
#include<GL/glew.h>
#include<boost/filesystem.hpp>
#include"glm/mat3x2.hpp"
#include"glm/vec2.hpp"
#include "../../../deps/vmath/src/virintox/math/Vector2.h"

namespace Graphics {
    struct TextureInfo{
        GLenum m_internalFormat;
        GLenum m_format;
        GLenum m_type;

        bool operator==(const TextureInfo& Obj) const;
    };

    class Texture {
    public:
        explicit Texture(const Math::Vector2i &size, const TextureInfo &info);
        explicit Texture(boost::filesystem::path PngFile);

        Texture(const Texture&) = delete;
        Texture& operator = (const Texture&) = delete;

        ~Texture();

        void setSize(Math::Vector2i);

        void setActive(unsigned i = 0) const;

        [[nodiscard]] GLuint getId() const;

        explicit operator GLuint() const;

        [[nodiscard]] Math::Vector2i getSize() const;

    protected:

        Math::Vector2i m_size;

        TextureInfo m_info;
        GLuint m_texId;
    };
}
#endif //PNS_TEXTURE_H
