#define GLEW_STATIC
#include<GL/glew.h>
#include<virintox/gcore/Viewport.h>
#include<virintox/gcore/Shader.h>
#include<imgui.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/string_cast.hpp>
#include<iostream>

Graphics::Viewport::Viewport(){
    framewidth = 0;
    frameheight = 0;

    glGenFramebuffers(1, &Framebuffer);
    glGenTextures(1, &FramebufferTex);
    glGenTextures(1, &FramebufferDepthTex);
    glGenTextures(1, &FramebufferMatIdTex);
    glGenTextures(1, &FramebufferWorldPosTex);
    RecreateFramebuffer();
}



void Graphics::Viewport::HandleInput(InputEvent) {/* Not a pure virtual */}

void Graphics::Viewport::RecreateFramebuffer() {

    /*glDeleteFramebuffers(1,&Framebuffer);
    glDeleteTextures(1,&Framebuffer);
    glDeleteTextures(1,&FramebufferDepthTex);
    glDeleteTextures(1,&FramebufferMatIdTex);
    glDeleteTextures(1,&FramebufferWorldPosTex);

    glGenFramebuffers(1, &Framebuffer);
    glGenTextures(1, &FramebufferTex);
    glGenTextures(1, &FramebufferDepthTex);
    glGenTextures(1, &FramebufferMatIdTex);
    glGenTextures(1, &FramebufferWorldPosTex);*/

    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    glBindTexture(GL_TEXTURE_2D, FramebufferTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framewidth, frameheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FramebufferTex, 0);

    glBindTexture(GL_TEXTURE_2D, FramebufferMatIdTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI , framewidth, frameheight, 0,  GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, FramebufferMatIdTex, 0);

    glBindTexture(GL_TEXTURE_2D, FramebufferWorldPosTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, framewidth, frameheight, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, FramebufferWorldPosTex, 0);

    glBindTexture(GL_TEXTURE_2D, FramebufferDepthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, framewidth, frameheight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, FramebufferDepthTex, 0);

    glBindTexture(GL_TEXTURE_2D,0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    lastframeheight = frameheight;
    lastframewidth = framewidth;
}

void Graphics::Viewport::ChangeView(const glm::vec2 &pos, float scale) const{
    auto matrix = glm::ortho(pos.x,pos.x+framewidth*scale,pos.y+frameheight*scale,pos.y);
    GLuint MatrixID = glGetUniformLocation(GetDefaultShader2D().ShaderId, "ProjectionMatrix");
    glUniformMatrix4fv(MatrixID,1,GL_FALSE,&matrix[0][0]);
}


void Graphics::Viewport::ChangeView(float x,float y, float scale) const{
    auto matrix = glm::ortho(x,x+framewidth*scale,y,y+frameheight*scale);
    GLuint MatrixID = glGetUniformLocation(GetDefaultShader2D().ShaderId, "ProjectionMatrix");
    glUniformMatrix4fv(MatrixID,1,GL_FALSE,&matrix[0][0]);

}
