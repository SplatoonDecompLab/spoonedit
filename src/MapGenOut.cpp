//
// Created by tv on 01.10.23.
//
#include<virintox/gcore/Graphics.h>
#include "virintox/gcore/Shader.h"
#include "virintox/gcore/Texture.h"
#include "virintox/gcore/Framebuffer.h"
#include"InstanceVars.h"
#include "glm/mat4x4.hpp"
#include "MainViewport.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "virintox/gcore/gui/FileSelectDialog.h"
#include<imgui.h>
#include<boost/filesystem.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<virintox/gcore/stb_image_write.h>

MENU_ITEM(Widgets,MapGeneration){
    Graphics::window->WidgetByName.find("MapGen")->second->Active ^= true;
}

MENU_ITEM(Object, Duenschiss){
    for(auto iter = loadedMap.Objects.begin(); iter != loadedMap.Objects.end(); iter++){
        if(iter->Type == "Enm_Takodozer"){
            auto itertemp = iter;
            iter--;
            loadedMap.Objects.erase(itertemp);
            selectedElem = nullptr;
        }

        if(iter->Layer == "Tmp"){
            auto itertemp = iter;
            iter--;
            loadedMap.Objects.erase(itertemp);
            selectedElem = nullptr;
        }
    }
}

struct Color{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

class MapGenWidget: public Graphics::Widget {
public:
    VCLASS(MapGenWidget,ESC({&Graphics::Widget::classInfo,nullptr}))
    MapGenWidget(): Graphics::Widget("MapGen"), m_shader("MapGen"), m_shaderPP("MapGenPP") , m_maptex(boost::filesystem::current_path() / "Models" / "Maptex.png"),
                    m_quadMesh({
                        {-1,-1},
                        {1 ,-1},
                        {-1, 1},
                        { 1, 1},
                    },{
                        2,1,0,
                        2,3,1,
                    }){
        g_instance = this;


        setup();

    };


    std::unique_ptr<Graphics::Framebuffer> m_framebuffer;
    Graphics::Shader m_shader;
    Graphics::Texture m_maptex;
    //PP = post processing
    std::unique_ptr<Graphics::Framebuffer> m_framebufferPP;
    Graphics::Shader m_shaderPP;
    Graphics::Mesh m_quadMesh;
    glm::mat4 m_camMatrix = glm::mat4(1);
    int m_size = 1600;
    int m_imgSize = 1600;
    void drawTex(){
        m_framebuffer->bind();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        GLint maptexLoc = m_shader.getUniformLocation("MapTex");
        glUniform1i(maptexLoc,6);

        m_maptex.setActive(6);

        for (const auto &item: loadedMap.Objects){
            if(item.Layer == "Tmp")
                continue;
            if(!item.m_isStatic)
                continue;

            auto mdl = g_getMainViewport()->MdlFromObj.find(item.Type);
            if(mdl == g_getMainViewport()->MdlFromObj.end())
                continue;

            mdl->second.Draw(item.TF,m_shader,m_camMatrix);
        }

        m_framebufferPP->bind();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        m_framebuffer->getFbTexByAttachment(GL_COLOR_ATTACHMENT0).m_texture->setActive(0);

        m_quadMesh.Draw();
    }
private:

    void setup(){
        m_framebuffer = std::make_unique<Graphics::Framebuffer>(
                std::vector<Graphics::FramebufferTextureInfo>{
                        {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0},
                        //{GL_R, GL_RED_INTEGER, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0}
                },
                m_shader,
                Math::Vector2i{1600,1600}
        );

        m_framebufferPP = std::make_unique<Graphics::Framebuffer>(
                std::vector<Graphics::FramebufferTextureInfo>{
                        {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0},
                        //{GL_R, GL_RED_INTEGER, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0}
                },
                m_shaderPP,
                Math::Vector2i{m_size,m_size}
        );

        //m_camMatrix = {
        //        {1/500,0,0,0},
        //        {0,0,0,0},
        //        {0,1/500,0,0},
        //        {0,0,0,1}
        //};

        float dist = (float)m_size / 1.6;

        glm::mat4 orthoProj = glm::ortho(
                -dist,dist,
                -dist,dist,0.0f,10000.0f
        );

        glm::mat4 viewMat = glm::lookAt(glm::vec3{0,5000.0,0},glm::vec3{0,0,0},glm::vec3{0,0,-1.0});

        GLint maptexLoc = m_shader.getUniformLocation("MapTex");
        glUniform1i(maptexLoc,6);

        GLint pptexLoc = m_shaderPP.getUniformLocation("Tex");
        glUniform1i(pptexLoc,0);

        m_camMatrix = orthoProj * viewMat;

        std::unique_ptr<Graphics::Texture> &fbTex = m_framebuffer->getFbTexByAttachment(GL_COLOR_ATTACHMENT0).m_texture;

        fbTex->setActive();

        float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        m_imgSize = m_size;
    }

    void Draw() override{
        drawTex();

        m_size = std::max(1,m_size);

        ImGui::InputInt("Size", &m_size);
        ImGui::SameLine();
        if(ImGui::Button("Update Size")){
            setup();
        }

        if(ImGui::Button("Export Map Texture##ExpMapTexBtn")){
            Graphics::FileSelectDialog* fsDiag = static_cast <Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Export Map Texture")->second);
            fsDiag->fileSelectMode = Graphics::Save;
            fsDiag->Active = true;
        }

        ImGui::Image(reinterpret_cast<ImTextureID>((m_framebufferPP->getFbTexByAttachment(GL_COLOR_ATTACHMENT0))
                .m_texture->getId()), ImVec2{(float)m_imgSize,(float)m_imgSize});



        glBindFramebuffer(GL_FRAMEBUFFER,0);

    };
public:
    static inline MapGenWidget* g_instance;
};

REGISTERVCLASS(MapGenWidget)

VGINIT_ACTION(AddMapGenFileselect){
    Graphics::window->addWidget(new Graphics::FileSelectDialog("Export Map Texture", {".png"},[&](boost::filesystem::path path){
        //Intentionally left empty
    },[&](boost::filesystem::path path){
        std::unique_ptr<Graphics::Framebuffer> &fb = MapGenWidget::g_instance->m_framebufferPP;

        fb->bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        std::vector<Color> m_colorData(MapGenWidget::g_instance->m_imgSize*MapGenWidget::g_instance->m_imgSize);

        glReadPixels(0, 0, MapGenWidget::g_instance->m_imgSize, MapGenWidget::g_instance->m_imgSize, GL_RGBA, GL_UNSIGNED_BYTE, &m_colorData[0].r);

        std::string str = path.string();

        stbi_write_png(str.c_str(),MapGenWidget::g_instance->m_imgSize,MapGenWidget::g_instance->m_imgSize,4,m_colorData.data(),4*MapGenWidget::g_instance->m_imgSize);
    }));
}