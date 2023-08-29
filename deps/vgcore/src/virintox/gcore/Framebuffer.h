#ifndef PNS_VIEWPORT_H
#define PNS_VIEWPORT_H
#include<GL/glew.h>
#include"glm/vec2.hpp"
#include"virintox/math/Vector2.h"
#include<memory>
#include "Texture.h"
#include"Shader.h"

enum InputType{
    MouseDownR,
    MouseDownL,
    MouseUpR,
    MouseUpL,
    MouseHoldR,
    MouseHoldL,
    MouseHover,
    MWheel
};

struct InputEvent{
    float x;
    float y;
    InputType EventType;
    float MWheel;
};

namespace Graphics {
    struct FramebufferTextureInfo{
        inline FramebufferTextureInfo(GLenum internalFormat,GLenum format,GLenum type,GLuint attachment){
            m_texInfo = {internalFormat,format,type};
            m_attachment = attachment;
        }

        GLuint m_attachment;
        TextureInfo m_texInfo;

        inline operator TextureInfo(){
            return m_texInfo;
        }
    };

    struct FramebufferTexture{
        inline FramebufferTexture(const Math::Vector2i &size,FramebufferTextureInfo texInfo): m_info(texInfo){
            m_texture = std::make_unique<Texture>(size,m_info.m_texInfo);
        }

        std::unique_ptr<Texture> m_texture;
        FramebufferTextureInfo m_info;
    };

    ///Returns true if the left hand side at least contains all of the targets specified in the right hand side
    bool operator==(const std::vector<FramebufferTextureInfo> &lhs, const std::vector<FramebufferTextureInfo> &rhs);
    bool operator==(const std::vector<FramebufferTexture> &lhs, const std::vector<FramebufferTextureInfo> &rhs);

    class Framebuffer{
    public:
        explicit Framebuffer(const std::vector<FramebufferTextureInfo> &buffers, const Shader &shader, const Math::Vector2i &size = {0,0});

        [[nodiscard]] Math::Vector2i getSize() const;

        void changeSize(Math::Vector2i size);

        void regenerateBuffers();

        void setBuffers();

        void bind();

        [[nodiscard]] GLuint getId();

        [[nodiscard]] FramebufferTexture& getFbTexByAttachment(GLuint attachment);

        Shader m_shader;

    private:
        void allocateTextures();
        void reallocateTextures();
        void bindTextures();


        GLuint m_framebufferId;

        std::vector<std::unique_ptr<FramebufferTexture>> m_targets;
        const std::vector<FramebufferTextureInfo> m_targetInfo;

        Math::Vector2i m_size;
    };
}
#endif //PNS_VIEWPORT_H
