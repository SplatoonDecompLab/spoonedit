//
// Created by tv on 21.05.23.
//

#include "MainViewport.h"
#include <glm/ext.hpp>
#include<imgui.h>
#include "IconsFontAwesome6.h"
#include "glm/gtx/string_cast.hpp"
#include<algorithm>
#include<cmath>
#include"Config/Configs.h"
#include "InstanceVars.h"
#include<virintox/gcore/Graphics.h>

MainViewport *MainVP;

glm::mat4 TransformToMatrix(Transform tf) {
    auto Pos = glm::vec3(tf.Position.X, tf.Position.Y, tf.Position.Z);
    auto Rot = glm::vec3(tf.Rotation.X, tf.Rotation.Y, tf.Rotation.Z);
    auto Scale = glm::vec3(tf.Scale.X, tf.Scale.Y, tf.Scale.Z);

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), Pos);
    glm::mat4 rotationMatrixZ = glm::rotate(glm::mat4(1), glm::radians(Rot.z), glm::vec3(0, 0, 1));
    glm::mat4 rotationMatrixY = glm::rotate(rotationMatrixZ, glm::radians(Rot.y), glm::vec3(0, 1, 0));
    glm::mat4 rotationMatrix = glm::rotate(rotationMatrixY, glm::radians(Rot.x), glm::vec3(1, 0, 0));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), Scale);

    return translationMatrix * rotationMatrix * scaleMatrix;
}

glm::vec3 ToGlmVec3(Vector3 Vec3) {
    return {Vec3.X, Vec3.Y, Vec3.Z};
}

bool IsArea(const std::string& Type) {
    return std::any_of(Configs::g_areas.begin(), Configs::g_areas.end(),[&](const std::string &areatype){
        return Type == areatype;
    });
}

MENU_ITEM_DISPLAY(Widgets,Viewport,ICON_FA_VIDEO " Viewport"){
    Graphics::window->WidgetByName.find("Main Viewport")->second->Active ^= true;
}

MainViewport::MainViewport() : Graphics::ViewportWidget("Main Viewport", true), VP(1.0f), defaultShader("ForwardPass"), flatShader("FlatForwardPass") {

    MainVP = this;

    m_internalFramebuf = std::make_unique<Graphics::Framebuffer>(
            std::vector<Graphics::FramebufferTextureInfo>{
                {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0},
                {GL_R32I, GL_RED_INTEGER, GL_INT, GL_COLOR_ATTACHMENT1},
                {GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_COLOR_ATTACHMENT2},
                {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, GL_DEPTH_STENCIL_ATTACHMENT}
            },
            defaultShader);

    SunCam = std::make_unique<Graphics::Framebuffer>(
            std::vector<Graphics::FramebufferTextureInfo>{
                    {GL_DEPTH_COMPONENT16,GL_DEPTH_COMPONENT,GL_FLOAT,GL_DEPTH_ATTACHMENT}
            },
            Graphics::Shader("ShadowPass"),
            Math::Vector2i(8192,8192));

    auto& tex = SunCam->getFbTexByAttachment(GL_DEPTH_ATTACHMENT);
    glBindTexture(GL_TEXTURE_2D, tex.m_texture->getId());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D,0);


    //ObjMesh = Graphics::Mesh();

    ObjIdPos = m_internalFramebuf->m_shader.getUniformLocation("ObjIdIn");
    ObjColPos = m_internalFramebuf->m_shader.getUniformLocation("ObjColor");
    TeamId = m_internalFramebuf->m_shader.getUniformLocation("TeamId");
    CamPosLoc = m_internalFramebuf->m_shader.getUniformLocation("CameraPosition");
    LightDirId = m_internalFramebuf->m_shader.getUniformLocation("LightDirection");
    SunVpLoc = m_internalFramebuf->m_shader.getUniformLocation("SunVP");
    distribLoc = m_internalFramebuf->m_shader.getUniformLocation("distribution");
    shadHardLoc = m_internalFramebuf->m_shader.getUniformLocation("shadowHardness");

    //Todo: update
    camFwdDir = flatShader.getUniformLocation("CamFwdDir");

}

void MainViewport::RenderRail(Rail *rail) {
    static Model RailPointMdl = Model("St_RailPoint");

    if (rail->Points.size() > 1) {
        auto lastRailPoint = rail->Points.front();

        for (RailPoint &railPoint: rail->Points) {
            RailPointMdl.Draw(railPoint.TF, m_internalFramebuf->m_shader, VP);
        }
    }
}

void MainViewport::Draw() {


    if(greenScreen) {
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    m_internalFramebuf->m_shader.use();




    static auto lasttime = boost::chrono::high_resolution_clock::now();

    auto timenow = boost::chrono::high_resolution_clock::now();

    boost::chrono::duration<float> TimeDiff = (timenow - lasttime);

    lasttime = timenow;

    float DeltaTime = TimeDiff.count();

    glm::vec3 cameraDirection = glm::vec3(
            glm::sin(glm::radians(camrot.x)) * glm::cos(glm::radians(camrot.y)),
            glm::sin(glm::radians(camrot.y)),
            glm::cos(glm::radians(camrot.x)) * glm::cos(glm::radians(camrot.y))
    );

    glm::vec3 cameraLeft = glm::vec3(
            glm::sin(glm::radians(camrot.x + 90)),
            0,
            glm::cos(glm::radians(camrot.x + 90))
    );

    if (ImGui::IsWindowFocused()) {
        if (ImGui::IsKeyPressed(ImGuiKey_N))
            ImGui::OpenPopup("Main VP Debug");

        float speedmod = 1.0f;

        if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
            speedmod = 2;
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
            speedmod = 0.5;

        if (ImGui::IsKeyDown(ImGuiKey_W)) {
            camPos += cameraDirection * speed * speedmod * DeltaTime;
        }
        if (ImGui::IsKeyDown(ImGuiKey_A)) {
            camPos += cameraLeft * speed * speedmod * DeltaTime;
        }

        if (ImGui::IsKeyDown(ImGuiKey_S)) {
            camPos -= cameraDirection * speed * speedmod * DeltaTime;
        }

        if (ImGui::IsKeyDown(ImGuiKey_D)) {
            camPos -= cameraLeft * speed * speedmod * DeltaTime;
        }
    }
    //glm::mat4 VP = Projection * View;


    glm::mat4 projectionMatrix = glm::perspective(
            glm::radians(fov),
            (float) m_internalFramebuf->getSize().x /
            (float) m_internalFramebuf->getSize().y,
            NearClippingPlane,
            FarClippingPlane
    );

    //glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), -camPos);
    glm::mat4 ViewMatrix = glm::lookAt(camPos, camPos + cameraDirection, glm::vec3(0, 1, 0));

    VP = projectionMatrix * ViewMatrix;// * ViewMatrix * ModelMat;



    if (ImGui::BeginPopup("Main VP Debug")) {
        ImGui::DragFloat3("Camera Postion", &camPos.x, 0.01);
        ImGui::DragFloat2("Camera Rotation", &camrot.x, 0.01);
        ImGui::SliderFloat("FOV", &fov, 0, 180);
        ImGui::DragFloat("Speed", &speed, 1);
        ImGui::DragFloat("Near Clipping Plane", &NearClippingPlane, 1);
        ImGui::DragFloat("Far Clipping Plane", &FarClippingPlane, 1);
        ImGui::DragFloat("Shadow Smoothing", &m_smoothingFactor, 1);
        ImGui::DragFloat("Shadow Hardness", &m_shadowHardness, 1);
        ImGui::DragFloat("Shadow Area", &shadowArea, 0.01);
        //ImGui::SliderFloat("Shading Effectiveness", &ShadingEffectiveness, 0, 1);
        ImGui::DragFloat3("Light Direction", &LightDir.x, 0.01);
        ImGui::Checkbox("Draw All Areas", &drawAllAreas);
        ImGui::Checkbox("Green Screen", &greenScreen);
        if(ImGui::Button("Switch Shading Modes")){
            if(flatShading)
                m_internalFramebuf->m_shader = defaultShader;
            else
                m_internalFramebuf->m_shader = flatShader;


            ObjIdPos = m_internalFramebuf->m_shader.getUniformLocation("ObjIdIn");
            ObjColPos = m_internalFramebuf->m_shader.getUniformLocation("ObjColor");
            TeamId = m_internalFramebuf->m_shader.getUniformLocation("TeamId");
            CamPosLoc = m_internalFramebuf->m_shader.getUniformLocation("CameraPosition");
            LightDirId = m_internalFramebuf->m_shader.getUniformLocation("LightDirection");
            SunVpLoc = m_internalFramebuf->m_shader.getUniformLocation("SunVP");

            flatShading = !flatShading;
        }
        ImGui::EndPopup();
    }

    glUniform3f(CamPosLoc, camPos.x,camPos.y,camPos.z);

    glUniform1f(distribLoc, m_smoothingFactor);
    glUniform1f(shadHardLoc, m_shadowHardness);

    m_internalFramebuf->m_shader.use();


    if(!flatShading)
        glUniform3f(LightDirId, LightDir.x , LightDir.y,LightDir.z);

    glUniform4f(ObjColPos, 1, 1, 1, 1);


    glUniform3f(camFwdDir, cameraDirection.x, cameraDirection.y, cameraDirection.z);

    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    GLuint ClearID[] = {0};
    glClearBufferuiv(GL_COLOR, 0, ClearID);

    glDrawBuffer(GL_COLOR_ATTACHMENT2);
    GLfloat ClearPos[] = {0, 0, 0, 0};
    glClearBufferfv(GL_COLOR, 0, ClearPos);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    unsigned int ObjID = 16;

    if(!flatShading) {

        glm::vec3 SunPos = glm::normalize(LightDir) * 2000.0f;

        glm::mat4 SunProj = glm::ortho(-shadowArea, shadowArea, -shadowArea, shadowArea, 500.0f, 3000.0f );

        glm::mat4 SunView = glm::lookAt(SunPos, {0, 0, 0}, {0, 1, 0});

        glm::mat4 SunVP = SunProj * SunView;

        SunCam->bind();
        SunCam->m_shader.use();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glDisable(GL_BLEND);

        glClear(GL_DEPTH_BUFFER_BIT);

        for (auto &obj: loadedMap.Objects) {
            if (IsArea(obj.Type)) {
                continue;
            }

            if (!MdlFromObj.contains(obj.Type)) {
                if (boost::filesystem::exists(
                        boost::filesystem::current_path() / "Models" / obj.Type / (obj.Type + ".dae")))
                    MdlFromObj.insert({obj.Type, Model(obj.Type)});
                else {
                    MdlFromObj.insert({obj.Type, Model("St_Default")});
                    std::cout << "Model missing: \n\t" << obj.Type << std::endl;
                }
            }


            auto &mdl = MdlFromObj.find(obj.Type)->second;

            mdl.Draw(obj.TF, SunCam->m_shader, SunVP);
        }

        m_internalFramebuf->bind();
        m_internalFramebuf->m_shader.use();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        auto &tex = SunCam->getFbTexByAttachment(GL_DEPTH_ATTACHMENT);
        tex.m_texture->setActive(5);



        glUniformMatrix4fv(SunVpLoc,1,GL_FALSE,&SunVP[0][0]);
    }



    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


    for (auto &obj: loadedMap.Objects) {
        glUniform1i(TeamId,(GLint)obj.Team);

        if (IsArea(obj.Type) && !drawAllAreas) {
            ObjID++;
            continue;
        } else if (IsArea(obj.Type) && drawAllAreas) {
            static auto AreaMdl = Model("St_Area");
            glUniform1i(ObjIdPos, ObjID);
            AreaMdl.Draw(obj.TF, m_internalFramebuf->m_shader, VP);
            ObjID++;
            continue;
        }

        if (!MdlFromObj.contains(obj.Type)) {
            if (boost::filesystem::exists(
                    boost::filesystem::current_path() / "Models" / obj.Type / (obj.Type + ".dae")))
                MdlFromObj.insert({obj.Type, Model(obj.Type)});
            else {
                MdlFromObj.insert({obj.Type, Model("St_Default")});
                std::cout << "Model missing: \n\t" << obj.Type << std::endl;
            }
        }


        auto &mdl = MdlFromObj.find(obj.Type)->second;
        glUniform1i(ObjIdPos, ObjID);

        if (&obj == selectedElem)
            glUniform4f(ObjColPos, 1, .7, .7, 1);

        mdl.Draw(obj.TF, m_internalFramebuf->m_shader, VP);
        if (&obj == selectedElem)
            glUniform4f(ObjColPos, 1, 1, 1, 1);

        ObjID++;
    }


    if (selectedElem != nullptr) {
        auto selobj = selectedElem;
        glUniform1i(ObjIdPos, 0);

        if (IsArea(selobj->Type)) {
            static auto AreaMdl = Model("St_Area");

            AreaMdl.Draw(selobj->TF, m_internalFramebuf->m_shader, VP);
        }

        glClear(GL_DEPTH_BUFFER_BIT);

        for (Link &link: selectedElem->Links) {
            Element *elem = loadedMap.GetElementById(link.Destination);
            if (elem == nullptr)
                continue;

            if (Rail *rail = dynamic_cast<Rail *>(elem)) {
                RenderRail(rail);
            }
        }

        if (Rail *rail = dynamic_cast<Rail *>(selectedElem)) {
            RenderRail(rail);
        }

        //mdl.DrawSelection(selobj->TF);

        glClear(GL_DEPTH_BUFFER_BIT);

        if (CurrentGizmoType == Move) {
            static Model Arrow = Model("St_Arrow");

            auto tf = Transform();
            tf.Position = selobj->TF.Position;
//            float size = glm::distance(glm::vec3(tf.Position.X, tf.Position.Y, tf.Position.Z), camPos);
            //tf.Scale = {size/ 50, size/ 50, size/ 50};
            tf.Scale = {1, 1, 1};
            tf.Rotation = {0, 0, 90};

            glUniform4f(ObjColPos, 0.5, 0, 0, 1);
            if (HoveredObjId == 1)
                glUniform4f(ObjColPos, 1, 0, 0, 1);
            glUniform1i(ObjIdPos, 1);
            Arrow.Draw(tf, m_internalFramebuf->m_shader, VP);

            tf.Rotation = {0, 0, 0};
            glUniform4f(ObjColPos, 0, 0.5, 0, 1);
            if (HoveredObjId == 2)
                glUniform4f(ObjColPos, 0, 1, 0, 1);
            glUniform1i(ObjIdPos, 2);
            Arrow.Draw(tf, m_internalFramebuf->m_shader, VP);

            tf.Rotation = {90, 0, 0};
            glUniform4f(ObjColPos, 0, 0, 0.5, 1);
            if (HoveredObjId == 3)
                glUniform4f(ObjColPos, 0, 0, 1, 1);
            glUniform1i(ObjIdPos, 3);
            Arrow.Draw(tf, m_internalFramebuf->m_shader, VP);

            glUniform4f(ObjColPos, 1, 1, 1, 1);
        } else if (CurrentGizmoType == Scale) {
            static Model Scalar = Model("St_Scalar");

            auto tf = Transform();
            tf.Position = selobj->TF.Position;
//            float size = glm::distance(glm::vec3(tf.Position.X, tf.Position.Y, tf.Position.Z), camPos);
            //tf.Scale = {size/ 50, size/ 50, size/ 50};
            tf.Scale = {1, 1, 1};
            tf.Rotation = {selobj->TF.Rotation.X + 90, selobj->TF.Rotation.Y + 90, selobj->TF.Rotation.Z};

            glUniform4f(ObjColPos, 0.5, 0, 0, 1);
            if (HoveredObjId == 1)
                glUniform4f(ObjColPos, 1, 0, 0, 1);
            glUniform1i(ObjIdPos, 1);
            Scalar.Draw(tf, m_internalFramebuf->m_shader, VP);

            tf.Rotation = {selobj->TF.Rotation.X, selobj->TF.Rotation.Y, selobj->TF.Rotation.Z};
            glUniform4f(ObjColPos, 0, 0.5, 0, 1);
            if (HoveredObjId == 2)
                glUniform4f(ObjColPos, 0, 1, 0, 1);
            glUniform1i(ObjIdPos, 2);
            Scalar.Draw(tf, m_internalFramebuf->m_shader, VP);

            tf.Rotation = {selobj->TF.Rotation.X + 90, selobj->TF.Rotation.Y, selobj->TF.Rotation.Z};
            glUniform4f(ObjColPos, 0, 0, 0.5, 1);
            if (HoveredObjId == 3)
                glUniform4f(ObjColPos, 0, 0, 1, 1);
            glUniform1i(ObjIdPos, 3);
            Scalar.Draw(tf, m_internalFramebuf->m_shader, VP);

            glUniform4f(ObjColPos, 1, 1, 1, 1);
        } else if (CurrentGizmoType == Rotate) {
            static Model Rotator = Model("St_Rotate");

            auto tf = Transform();
            tf.Position = selobj->TF.Position;
//            float size = glm::distance(glm::vec3(tf.Position.X, tf.Position.Y, tf.Position.Z), camPos);
            //tf.Scale = {size/ 50, size/ 50, size/ 50};
            tf.Scale = {1, 1, 1};
            tf.Rotation = {0, 0, 90};

            glUniform4f(ObjColPos, 0.5, 0, 0, 1);
            if (HoveredObjId == 1)
                glUniform4f(ObjColPos, 1, 0, 0, 1);
            glUniform1i(ObjIdPos, 1);
            Rotator.Draw(tf, m_internalFramebuf->m_shader, VP);

            tf.Rotation = {0, 0, 0};
            glUniform4f(ObjColPos, 0, 0.5, 0, 1);
            if (HoveredObjId == 2)
                glUniform4f(ObjColPos, 0, 1, 0, 1);
            glUniform1i(ObjIdPos, 2);
            Rotator.Draw(tf, m_internalFramebuf->m_shader, VP);

            tf.Rotation = {90, 0, 0};
            glUniform4f(ObjColPos, 0, 0, 0.5, 1);
            if (HoveredObjId == 3)
                glUniform4f(ObjColPos, 0, 0, 1, 1);
            glUniform1i(ObjIdPos, 3);
            Rotator.Draw(tf, m_internalFramebuf->m_shader, VP);

            glUniform4f(ObjColPos, 1, 1, 1, 1);
        }


    }
}

template<typename t>
inline float GetVecComponent(const t &v1, const t &v2) {
    auto NormComp = glm::normalize(v2);
    return (glm::dot(v1, NormComp));
}

template<typename t>
inline t ProjectToRayNormalized(const t &v1, const t &ray) {
    return glm::dot(v1, ray) / glm::dot(ray, ray) * ray;
}

template<typename t>
inline t ProjectToRay(const t &v1, const t &rayStart, const t &ray) {
    return ProjectToRayNormalized(v1 - rayStart, ray - rayStart) + rayStart;
}

template<typename t>
inline t ProjectToRayNormDir(const t &v1, const t &rayStart, const t &RayDir) {
    return ProjectToRayNormalized(v1 - rayStart, RayDir) + rayStart;
}

glm::vec3 MainViewport::ScreenSpaceToRay(glm::vec2 ScreenPos) {
    auto InverseVP = glm::inverse(VP);

    glm::vec4 worldPos = InverseVP * glm::vec4(ScreenPos, 1.0, 1.0);

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}

void MainViewport::HandleInput(InputEvent event) {
    static glm::vec2 lastMpos = glm::vec2(0, 0);

    glm::vec2 currentMpos = glm::vec2(event.x, event.y);

    //static glm::vec3 GizmoDirPrePos = {0,0,0};

    glm::vec2 DeltaMousePos = currentMpos - lastMpos;

    static glm::vec3 ObjBindingVec = {0, 0, 0};
    static float ObjScalingDist = 0.0f;

    static glm::vec3 GizmoDir = glm::vec3(1, 1, 1);
    static unsigned int SpecialObjCur = 0;

    auto SelObj = selectedElem;

    switch (event.EventType) {
        case (InputType::MouseHover): {
            GLint y = event.y * m_internalFramebuf->getSize().y;
            GLint x = event.x * m_internalFramebuf->getSize().x;

            glReadBuffer(GL_COLOR_ATTACHMENT1);
            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &HoveredObjId);
        }
            break;
        case (InputType::MouseDownL): {
            GLint y = event.y * m_internalFramebuf->getSize().y;
            GLint x = event.x * m_internalFramebuf->getSize().x;

            glBindFramebuffer(GL_FRAMEBUFFER, m_internalFramebuf->getId());
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            int obj = 0;

            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &obj);


            if (obj < 16) {
                SpecialObjCur = obj;
                if (CurrentGizmoType == Move) {
                    if (0 < obj && obj <= 3) {
                        if (obj == 1) {
                            GizmoDir = {1, 0, 0};
                        } else if (obj == 2) {
                            GizmoDir = {0, 1, 0};
                        } else if (obj == 3) {
                            GizmoDir = {0, 0, 1};
                        }

                        glReadBuffer(GL_COLOR_ATTACHMENT2);
                        glm::vec4 CurWDirPos = {0, 0, 0, 0};

                        glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(CurWDirPos));

                        glm::vec3 GizmoDirPrePos = ProjectToRayNormalized({CurWDirPos.x, CurWDirPos.y, CurWDirPos.z},
                                                                          GizmoDir);
                        glm::vec3 objpos = {SelObj->TF.Position.X, SelObj->TF.Position.Y, SelObj->TF.Position.Z};

                        ObjBindingVec = objpos - GizmoDirPrePos;

                        std::cout << glm::to_string(ObjBindingVec) << std::endl;
                    }
                } else if (CurrentGizmoType == Scale) {
                    if (0 < obj && obj <= 3) {
                        if (obj == 1) {
                            GizmoDir = {1, 0, 0};
                        } else if (obj == 2) {
                            GizmoDir = {0, 1, 0};
                        } else if (obj == 3) {
                            GizmoDir = {0, 0, 1};
                        }

                        auto TF = SelObj->TF;

                        TF.Scale = {1, 1, 1};
                        TF.Position = {0, 0, 0};

                        auto mat = TransformToMatrix(TF);

                        GizmoDir = mat * glm::vec4(GizmoDir, 1);

                        glReadBuffer(GL_COLOR_ATTACHMENT2);
                        glm::vec4 CurWDirPos = {0, 0, 0, 0};

                        glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(CurWDirPos));

                        glm::vec3 GizmoDirPrePos = ProjectToRayNormalized({CurWDirPos.x, CurWDirPos.y, CurWDirPos.z},
                                                                          GizmoDir);

                        glm::vec3 objpos = ToGlmVec3(SelObj->TF.Position);

                        ObjScalingDist = glm::distance(ProjectToRayNormalized(objpos, GizmoDir), GizmoDirPrePos);

                        ObjBindingVec = ToGlmVec3(SelObj->TF.Scale);

                        //std::cout << ObjScalingDist << std::endl;
                    } /*if (CurrentGizmoType == Rotate) {
                        if(0 < obj && obj <= 3 ) {
                            if (obj == 1) {
                                GizmoDir = {1, 0, 0};
                            } else if (obj == 2) {
                                GizmoDir = {0, 1, 0};
                            } else if (obj == 3) {
                                GizmoDir = {0, 0, 1};
                            }

                            glReadBuffer(GL_COLOR_ATTACHMENT2);
                            glm::vec4 CurWDirPos = {0,0,0,0};

                            glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(CurWDirPos));

                            glm::vec3 GizmoDirPrePos = ProjectToRayNormalized({CurWDirPos.x,-CurWDirPos.y,CurWDirPos.z},GizmoDir);
                            glm::vec3 objpos = {SelObj->TF.Position.X,SelObj->TF.Position.Y,SelObj->TF.Position.Z};

                            ObjBindingVec = objpos - GizmoDirPrePos;
                        }
                    }*/
                }

                return;
            }

            obj -= 16;

            auto iter = loadedMap.Objects.begin();

            while(obj != 0){
                iter++;
                obj--;
            }

            selectedElem = &*iter;
        }
            break;
        case (InputType::MouseHoldL): {
            GLint y = event.y * m_internalFramebuf->getSize().y;
            GLint x = event.x * m_internalFramebuf->getSize().x;

            glBindFramebuffer(GL_FRAMEBUFFER, m_internalFramebuf->getId());
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            int obj = 0;

            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &obj);

            if (CurrentGizmoType == Move) {
                if (SpecialObjCur == 0) {
                    return;
                }
                /*if(SpecialObjCur != obj){
                    SpecialObjCur = 0;
                    return;
                }*/

                auto tf = SelObj->TF;

                tf.Rotation = {0, 0, 0};
                tf.Scale = {1, 1, 1};

                auto Tf = TransformToMatrix(tf);

                if (SpecialObjCur == 1) {
                    GizmoDir = {1, 0, 0};
                } else if (SpecialObjCur == 2) {
                    GizmoDir = {0, 1, 0};
                } else if (SpecialObjCur == 3) {
                    GizmoDir = {0, 0, 1};
                }

                auto ClipSpaceStart = (VP * Tf) * glm::vec4(0, 0, 0, 1);
                auto ClipSpaceEnd = (VP * Tf) * glm::vec4(GizmoDir, 1);

                glm::vec2 ScreenSpaceStart = {
                        ((ClipSpaceStart.x / ClipSpaceStart.w + 1) / 2) * (float)m_internalFramebuf->getSize().x,
                        ((ClipSpaceStart.y / ClipSpaceStart.w + 1) / 2) * (float)m_internalFramebuf->getSize().y};
                glm::vec2 ScreenSpaceEnd = {
                        ((ClipSpaceEnd.x / ClipSpaceEnd.w + 1) / 2) * (float)m_internalFramebuf->getSize().x,
                        ((ClipSpaceEnd.y / ClipSpaceEnd.w + 1) / 2) * (float)m_internalFramebuf->getSize().y};

                glm::vec2 ScrenSpaceDir = ScreenSpaceEnd - ScreenSpaceStart;

                glm::vec2 MousePos = {x, y};

                glm::vec2 FixedMousePos = ProjectToRayNormDir(MousePos, ScreenSpaceStart, ScrenSpaceDir);

                //SpecialObjCur = obj;
                if (0 < SpecialObjCur && SpecialObjCur <= 3) {
                    glReadBuffer(GL_COLOR_ATTACHMENT2);
                    glm::vec4 CurWDirPos = {0, 0, 0, 0};

                    glReadPixels((int)floorf(FixedMousePos.x), (int)floorf(FixedMousePos.y), 1, 1, GL_RGBA, GL_FLOAT,
                                 glm::value_ptr(CurWDirPos));

                    //std::cout << glm::to_string(CurWDirPos) << std::endl;

                    auto DirProjectedTranslationPos = ProjectToRayNormalized(
                            {CurWDirPos.x, CurWDirPos.y, CurWDirPos.z}, GizmoDir);

                    if (SpecialObjCur == 1) {
                        SelObj->TF.Position.X = ObjBindingVec.x + DirProjectedTranslationPos.x;
                    } else if (SpecialObjCur == 2) {
                        SelObj->TF.Position.Y = ObjBindingVec.y + DirProjectedTranslationPos.y;
                    } else if (SpecialObjCur == 3) {
                        SelObj->TF.Position.Z = ObjBindingVec.z + DirProjectedTranslationPos.z;
                    }

                    glm::vec3 objpos = {SelObj->TF.Position.X, SelObj->TF.Position.Y, SelObj->TF.Position.Z};

                    ObjBindingVec = objpos - DirProjectedTranslationPos;
                    lastMpos = currentMpos;
                }
            } else if (CurrentGizmoType == Scale) {
                if (SpecialObjCur == 0)
                    return;
                if (SpecialObjCur != obj) {
                    SpecialObjCur = 0;
                    return;
                }
                if (0 < SpecialObjCur && SpecialObjCur <= 3) {
                    SpecialObjCur = obj;
                    if (obj == 1) {
                        GizmoDir = {1, 0, 0};
                    } else if (obj == 2) {
                        GizmoDir = {0, 1, 0};
                    } else if (obj == 3) {
                        GizmoDir = {0, 0, 1};
                    }

                    auto TF = SelObj->TF;

                    TF.Scale = {1, 1, 1};
                    TF.Position = {0, 0, 0};

                    auto mat = TransformToMatrix(TF);

                    GizmoDir = mat * glm::vec4(GizmoDir, 1);

                    glReadBuffer(GL_COLOR_ATTACHMENT2);
                    glm::vec4 CurWDirPos = {0, 0, 0, 0};

                    //GizmoDir = glm::normalize( * GizmoDir)
                    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(CurWDirPos));

                    glm::vec3 ObjPos = ToGlmVec3(SelObj->TF.Position);

                    ObjPos.y = -ObjPos.y;

                    CurWDirPos -= glm::vec4(ObjPos, 1);


                    auto DirProjectedTranslationPos = ProjectToRayNormalized(
                            {CurWDirPos.x, CurWDirPos.y, CurWDirPos.z}, GizmoDir);

                    float VecComp = GetVecComponent(DirProjectedTranslationPos, GizmoDir);

                    float DeltaInDir = 0;

                    if (SpecialObjCur == 1) {
                        DeltaInDir = (ObjBindingVec.x + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.x) -
                                     SelObj->TF.Scale.X;
                        SelObj->TF.Scale.X = ObjBindingVec.x + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.x;
                    } else if (SpecialObjCur == 2) {
                        DeltaInDir = (ObjBindingVec.y + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.y) -
                                     SelObj->TF.Scale.Y;
                        SelObj->TF.Scale.Y = ObjBindingVec.y + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.y;
                    } else if (SpecialObjCur == 3) {
                        DeltaInDir = (ObjBindingVec.z + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.z) -
                                     SelObj->TF.Scale.Z;
                        SelObj->TF.Scale.Z = ObjBindingVec.z + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.z;
                    }

                    if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
                        if (SpecialObjCur != 1) SelObj->TF.Scale.X += DeltaInDir;
                        if (SpecialObjCur != 2) SelObj->TF.Scale.Y += DeltaInDir;
                        if (SpecialObjCur != 3) SelObj->TF.Scale.Z += DeltaInDir;
                    }

                    lastMpos = currentMpos;
                }
            }
        }
            break;
        case (InputType::MouseUpL):
            SpecialObjCur = 0;
            break;

        case (InputType::MouseDownR):
            ImGui::SetWindowFocus();
            lastMpos = currentMpos;
            break;
        case (InputType::MouseHoldR):
            ImGui::SetWindowFocus();
            camrot.x += (lastMpos.x - currentMpos.x) * (float) m_internalFramebuf->getSize().x * .1f;
            camrot.y += (lastMpos.y - currentMpos.y) * (float) m_internalFramebuf->getSize().y * .1f;
            lastMpos = currentMpos;
            break;
        default:
            break;
    }

}

void MainViewport::DrawOver() {

    auto textsz = ImGui::CalcTextSize(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT "  "
                                      ICON_FA_ARROWS_LEFT_RIGHT_TO_LINE "  "
                                      ICON_FA_ROTATE);
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - textsz.x - 70, textsz.y + 20));
    ImGui::BeginGroupPanel("Tools");
    if (CurrentGizmoType == Move) {
        ImGui::PushStyleColor(ImGuiCol_Button, 0xFF008000);
        if (ImGui::Button(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT "##MainVpMoveGadget"))
            CurrentGizmoType = Move;
        ImGui::PopStyleColor();
    } else {
        if (ImGui::Button(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT "##MainVpMoveGadget"))
            CurrentGizmoType = Move;
    }

    ImGui::SameLine();

    if (CurrentGizmoType == Scale) {
        ImGui::PushStyleColor(ImGuiCol_Button, 0xFF008000);
        if (ImGui::Button(ICON_FA_ARROWS_LEFT_RIGHT_TO_LINE "##MainVpScaleGadget"))
            CurrentGizmoType = Scale;
        ImGui::PopStyleColor();
    } else {
        if (ImGui::Button(ICON_FA_ARROWS_LEFT_RIGHT_TO_LINE "##MainVpScaleGadget"))
            CurrentGizmoType = Scale;
    }

    ImGui::SameLine();

    if (CurrentGizmoType == Rotate) {
        ImGui::PushStyleColor(ImGuiCol_Button, 0xFF008000);
        if (ImGui::Button(ICON_FA_ROTATE "##MainVpRotateGadget"))
            CurrentGizmoType = Rotate;
        ImGui::PopStyleColor();
    } else {
        if (ImGui::Button(ICON_FA_ROTATE "##MainVpRotateGadget"))
            CurrentGizmoType = Rotate;
    }
    ImGui::EndGroupPanel();
}

glm::vec2 MainViewport::CalcGizmoDir(glm::vec3 dir) {


    auto DirPos = glm::vec4(dir, 1);
    auto ObjPos = glm::vec4(0, 0, 0, 1);


    auto tf = selectedElem->TF;

    tf.Rotation = {0, 0, 0};
    tf.Scale = {1, 1, 1};


    auto ModelMat = TransformToMatrix(tf);

    auto ObjPosClipSpace = (VP * ModelMat) * ObjPos;
    auto DirPosClipSpace = (VP * ModelMat) * DirPos;

    auto ObjPosVPSpace = glm::vec2(ObjPosClipSpace.x / ObjPosClipSpace.w,
                                   ObjPosClipSpace.y / ObjPosClipSpace.w);
    auto DirPosVPSpace = glm::vec2(DirPosClipSpace.x / DirPosClipSpace.w,
                                   DirPosClipSpace.y / DirPosClipSpace.w);

    return DirPosVPSpace - ObjPosVPSpace;
}

void MainViewport::clearModels() {
    MdlFromObj.clear();
}

void MainViewport::cleanUnnescessary() {
    for(auto mdlPairIter = MdlFromObj.begin(); mdlPairIter != MdlFromObj.end(); ){
        auto nextiter = mdlPairIter;
        nextiter++;

        if(nextiter == MdlFromObj.end())
            break;

        if(std::none_of(loadedMap.Objects.begin(), loadedMap.Objects.end(),[&](const LevelObject &obj){
            return obj.Type == nextiter->first;
        })){
            MdlFromObj.erase(nextiter);
        } else {
            mdlPairIter = nextiter;
        }
    }
}

MainViewport *g_getMainViewport() {
    return MainVP;
}

REGISTERVCLASS(MainViewport)