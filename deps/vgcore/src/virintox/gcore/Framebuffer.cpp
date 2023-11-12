#define GLEW_STATIC
#include<GL/glew.h>
#include"Framebuffer.h"
#include"Shader.h"
#include"imgui.h"
#include<algorithm>

namespace Graphics {

    Framebuffer::Framebuffer(const std::vector<FramebufferTextureInfo> &buffers, const Shader &shader, const Math::Vector2i &size) : m_size(size), m_targetInfo(buffers),
                                                                                                                                    m_shader(shader), m_targets() {
        glGenFramebuffers(1, &m_framebufferId);


        glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferId);
        allocateTextures();
        bindTextures();
    }

    void Framebuffer::bind() {
        glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferId);
        m_shader.use();
        glViewport(0, 0, m_size.x, m_size.y);
        setBuffers();
    }

    void Framebuffer::setBuffers() {
        std::vector<GLuint> Buffers(m_targets.size());

        unsigned i = 0;
        for(const std::unique_ptr<FramebufferTexture> &tex: m_targets){
            Buffers[i] = tex->m_info.m_attachment;
            i++;
        }

        glDrawBuffers(Buffers.size(),Buffers.data());
    }

    void Framebuffer::changeSize(Math::Vector2i size) {
        m_size = size;
        regenerateBuffers();
        bindTextures();
    }

    void Framebuffer::regenerateBuffers() {
        //deallocateTextures();
        reallocateTextures();
        bindTextures();
    }

    void Framebuffer::allocateTextures() {
        for(const FramebufferTextureInfo &texInfo: m_targetInfo){
            m_targets.push_back(std::make_unique<FramebufferTexture>(m_size, texInfo));
        }
    }

    void Framebuffer::bindTextures() {
        glBindFramebuffer(GL_FRAMEBUFFER,m_framebufferId);
        for(const std::unique_ptr<FramebufferTexture> &fbTex: m_targets){
            glBindTexture(GL_TEXTURE_2D, fbTex->m_texture->getId());
            glFramebufferTexture2D(GL_FRAMEBUFFER, fbTex->m_info.m_attachment, GL_TEXTURE_2D, fbTex->m_texture->getId(), 0);
        }
        glBindTexture(GL_TEXTURE_2D,0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    FramebufferTexture& Framebuffer::getFbTexByAttachment(GLuint attachment) {
        auto iter = std::find_if(m_targets.begin(), m_targets.end(),[&](const std::unique_ptr<FramebufferTexture> &Tex){
            return Tex->m_info.m_attachment == attachment;
        });

        assert((iter != m_targets.end()) && "Unable to find specified attachment!");

        return **iter;
    }

    Math::Vector2i Framebuffer::getSize() const {
        return m_size;
    }

    GLuint Framebuffer::getId() {
        return m_framebufferId;
    }

    void Framebuffer::reallocateTextures() {
        for(std::unique_ptr<FramebufferTexture>& tex: m_targets){
            tex->m_texture->setSize(m_size);
        }
    }
}