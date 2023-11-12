#include "Mesh.h"
#include "Graphics.h"
#include"Texture.h"
#include"Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include<iostream>

namespace Graphics {
    bool TextureInfo::operator==(const TextureInfo &Obj) const {
        if (Obj.m_format == m_format &&
            Obj.m_internalFormat == m_internalFormat &&
            Obj.m_type == m_type)
            return true;

        return false;
    }

    Texture::Texture(const Math::Vector2i &size, const TextureInfo &info): m_info(info) {
        glGenTextures(1, &m_texId);
        glBindTexture(GL_TEXTURE_2D, m_texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, info.m_internalFormat, size.x, size.y, 0, info.m_format, info.m_type, NULL);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    Texture::Texture(boost::filesystem::path PngFile) {
        glGenTextures(1, &m_texId);
        glBindTexture(GL_TEXTURE_2D, m_texId);

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

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        m_size = {width, height};

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    /*Texture::Texture(const Texture &tex): m_count(tex.m_count), m_texId(tex.m_texId), m_size(tex.m_size) {
        (*m_count)++;
    }

    Texture::~Texture() {
        (*m_count)--;

        if((*m_count) == 0) {
            delete m_count;
            //glDeleteTextures(1, &m_texId);
        }
    }
     */

    void Texture::setActive(unsigned i) const{
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_texId);
    }

    Math::Vector2i Texture::getSize() const {
        return m_size;
    }

    GLuint Texture::getId() const {
        return m_texId;
    }

    Texture::operator GLuint() const {
        return m_texId;
    }

    void Texture::setSize(Math::Vector2i size) {
        m_size = size;

        glBindTexture(GL_TEXTURE_2D, m_texId);

        glTexImage2D(GL_TEXTURE_2D, 0, m_info.m_internalFormat, size.x, size.y, 0, m_info.m_format, m_info.m_type, NULL);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_texId);
    }
}
