//
// Created by tv on 21.05.23.
//

#include "MainViewport.h"
#include "MainWindow.h"
#include <glm/ext.hpp>
#include<imgui.h>
#include "IconsFontAwesome6.h"
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/rotate_vector.hpp>

#include<cmath>

const GLchar *vertexShader = "#version 460\n"
                             "layout (location = 0) in vec3 aPos;\n"
                             "layout (location = 1) in vec2 aTexCoord;\n"
                             "uniform mat4 VP;\n"
                             "uniform mat4 TransformationMatrix;\n"
                             "uniform mat4 TransformationMatrixNoYInv;\n"
                             "out vec2 TexCoord;\n"
                             "out vec4 WldPos;\n"
                             "void main(){\n"
                             "    vec3 glPos = vec3(aPos.x,-aPos.y,aPos.z);\n"
                             "    WldPos = TransformationMatrixNoYInv * vec4(aPos.xyz,1);\n"
                             "    gl_Position = (VP * TransformationMatrix) * vec4(glPos.x,glPos.y,glPos.z,1);\n"
                             "    TexCoord = aTexCoord;\n"
                             "}\n";
const GLchar *fragmentShader = "#version 460\n"
                               "in vec2 TexCoord;\n"
                               "in vec4 WldPos;\n"
                               "layout(location = 1) out vec4 FragColor;\n"
                               "layout(location = 2) out unsigned int ObjId;\n"
                               "layout(location = 3) out vec4 WorldPos;\n"
                               "uniform sampler2D tex;\n"
                               "uniform unsigned int ObjIdIn;\n"
                               "uniform vec4 ObjColor;\n"
                               "//out vec4 CamPos;\n"
                               "void main(){\n"
                               "    ObjId = ObjIdIn;\n"
                               "    WorldPos = vec4(WldPos.xyz,1);\n"
                               "    //if(WldPos.w == 0) WorldPos=vec4(WldPos.xyz,1);\n"
                               "    vec4 textel = texture(tex,TexCoord);\n"
                               "    if(textel.a < 0.2) discard;\n"
                               "    //textel.a = (CamPos.z / CamPos.w) / 100;\n"
                               "    FragColor = textel * ObjColor;\n"
                               "    //FragColor = vec4(1,1,1,1);\n"
                               "}";

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
    return glm::vec3(Vec3.X, Vec3.Y, Vec3.Z);
}

bool IsArea(std::string Type){
    if( Type == "Area" ||
        Type == "Area_Yellow" ||
        Type == "StageArea" ||
        Type == "GeneralArea" ||
        Type == "PaintedArea" ||
        Type == "GachihokoHikikomoriArea" ||
        Type == "GachihokoHikikomoriArea2" ||
        Type == "SearchableArea" ||
        Type == "PaintTargetArea")
        return true;
    return false;
}

MainViewport::MainViewport() : Graphics::ViewportWidget("Main Viewport", true) {

    MainVP = this;
    shader = Graphics::Shader(vertexShader, fragmentShader);
    //ObjMesh = Graphics::Mesh();

    GLuint ObjColPos = glGetUniformLocation(shader.ShaderId, "ObjColor");
    glUniform4f(ObjColPos, 1, 1, 1, 1);

}

void MainViewport::Draw() {
    GLuint ObjIdPos = glGetUniformLocation(shader.ShaderId, "ObjIdIn");
    GLuint ObjColPos = glGetUniformLocation(shader.ShaderId, "ObjColor");


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
            glm::radians(
                    fov), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
            (float) framewidth /
            (float) frameheight,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
            0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
            10000.0f             // Far clipping plane. Keep as little as possible.
    );

    //glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), -camPos);
    glm::mat4 ViewMatrix = glm::lookAt(camPos, camPos + cameraDirection, glm::vec3(0, 1, 0));

    glm::mat4 ModelMat = glm::mat4(1.0f);

    VP = projectionMatrix * ViewMatrix;// * ViewMatrix * ModelMat;

    if (ImGui::BeginPopup("Main VP Debug")) {
        ImGui::DragFloat3("Camera Postion", &camPos.x, 0.01);
        ImGui::DragFloat2("Camera Rotation", &camrot.x, 0.01);
        ImGui::SliderFloat("FOV", &fov, 0, 180);
        ImGui::DragFloat("Speed", &speed, 1);
        ImGui::Checkbox("Draw All Areas", &drawAllAreas);
        ImGui::EndPopup();
    }

    GLuint MatrixID = glGetUniformLocation(shader.ShaderId, "VP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(VP));

    auto &map = GetMainWindow()->loadedMap;

    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    GLuint ClearID[] = {0};
    glClearBufferuiv(GL_COLOR, 0, ClearID);

    glDrawBuffer(GL_COLOR_ATTACHMENT2);
    GLfloat ClearPos[] = {0,0,0,0};
    glClearBufferfv(GL_COLOR, 0, ClearPos);

    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    unsigned int ObjID = 16;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    for (auto &obj: map.Objects) {
        if(IsArea(obj.Type) && !drawAllAreas){
            ObjID++;
            continue;
        } else if(IsArea(obj.Type) && drawAllAreas) {
            static auto AreaMdl = Model("St_Area");
            glUniform1ui(ObjIdPos, ObjID);
            glUniform4f(ObjColPos, 1, 1, 1, 1);
            AreaMdl.Draw(obj.TF);
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


        auto mdl = MdlFromObj.find(obj.Type)->second;
        glUniform1ui(ObjIdPos, ObjID);
        glUniform4f(ObjColPos, 1, 1, 1, 1);

        if(&obj == GetMainWindow()->selectedElem)
            glUniform4f(ObjColPos, 1, .7, .7, 1);

        mdl.Draw(obj.TF);

        ObjID++;
    }



    if (GetMainWindow()->selectedElem != nullptr) {
        auto selobj = GetMainWindow()->selectedElem;

        if(IsArea(selobj->Type)) {
            static auto AreaMdl = Model("St_Area");

            glUniform1ui(ObjIdPos, 0);

            AreaMdl.Draw(selobj->TF);
        }

        glClear(GL_DEPTH_BUFFER_BIT);

        if(Rail* rail = dynamic_cast<Rail*>(GetMainWindow()->selectedElem)){
            static Model Arrow = Model("St_RailPoint");

            if(rail->Points.size() > 1) {
                auto lastRailPoint = rail->Points.front();

                for (RailPoint &railPoint: rail->Points) {
                    Arrow.Draw(railPoint.TF);
                }
            }
        }

        //mdl.DrawSelection(selobj->TF);

        glClear(GL_DEPTH_BUFFER_BIT);

        if (CurrentGizmoType == Move) {
            static Model Arrow = Model("St_Arrow");

            auto tf = Transform();
            tf.Position = selobj->TF.Position;
            float size = glm::distance(glm::vec3(tf.Position.X,tf.Position.Y,tf.Position.Z),camPos);
            //tf.Scale = {size/ 50, size/ 50, size/ 50};
            tf.Scale = {1,1,1};
            tf.Rotation = {0, 0, 90};

            glUniform4f(ObjColPos, 0.5, 0, 0, 1);
            if(HoveredObjId == 1)
                glUniform4f(ObjColPos, 1, 0, 0, 1);
            glUniform1ui(ObjIdPos, 1);
            Arrow.Draw(tf);

            tf.Rotation = {0, 0, 0};
            glUniform4f(ObjColPos, 0, 0.5, 0, 1);
            if(HoveredObjId == 2)
                glUniform4f(ObjColPos, 0, 1, 0, 1);
            glUniform1ui(ObjIdPos, 2);
            Arrow.Draw(tf);

            tf.Rotation = {90, 0, 0};
            glUniform4f(ObjColPos, 0, 0, 0.5, 1);
            if(HoveredObjId == 3)
                glUniform4f(ObjColPos, 0, 0, 1, 1);
            glUniform1ui(ObjIdPos, 3);
            Arrow.Draw(tf);

            glUniform4f(ObjColPos, 1, 1, 1, 1);
        } else if (CurrentGizmoType == Scale) {
            static Model Scalar = Model("St_Scalar");

            auto tf = Transform();
            tf.Position = selobj->TF.Position;
            float size = glm::distance(glm::vec3(tf.Position.X,tf.Position.Y,tf.Position.Z),camPos);
            //tf.Scale = {size/ 50, size/ 50, size/ 50};
            tf.Scale = {1,1,1};
            tf.Rotation = {selobj->TF.Rotation.X + 90, selobj->TF.Rotation.Y + 90, selobj->TF.Rotation.Z};

            glUniform4f(ObjColPos, 0.5, 0, 0, 1);
            if(HoveredObjId == 1)
                glUniform4f(ObjColPos, 1, 0, 0, 1);
            glUniform1ui(ObjIdPos, 1);
            Scalar.Draw(tf);

            tf.Rotation = {selobj->TF.Rotation.X, selobj->TF.Rotation.Y, selobj->TF.Rotation.Z};
            glUniform4f(ObjColPos, 0, 0.5, 0, 1);
            if(HoveredObjId == 2)
                glUniform4f(ObjColPos, 0, 1, 0, 1);
            glUniform1ui(ObjIdPos, 2);
            Scalar.Draw(tf);

            tf.Rotation = {selobj->TF.Rotation.X + 90 , selobj->TF.Rotation.Y, selobj->TF.Rotation.Z};
            glUniform4f(ObjColPos, 0, 0, 0.5, 1);
            if(HoveredObjId == 3)
                glUniform4f(ObjColPos, 0, 0, 1, 1);
            glUniform1ui(ObjIdPos, 3);
            Scalar.Draw(tf);

            glUniform4f(ObjColPos, 1, 1, 1, 1);
        } else if (CurrentGizmoType == Rotate) {
            static Model Rotator = Model("St_Rotate");

            auto tf = Transform();
            tf.Position = selobj->TF.Position;
            float size = glm::distance(glm::vec3(tf.Position.X,tf.Position.Y,tf.Position.Z),camPos);
            //tf.Scale = {size/ 50, size/ 50, size/ 50};
            tf.Scale = {1,1,1};
            tf.Rotation = {0, 0, 90};

            glUniform4f(ObjColPos, 0.5, 0, 0, 1);
            if(HoveredObjId == 1)
                glUniform4f(ObjColPos, 1, 0, 0, 1);
            glUniform1ui(ObjIdPos, 1);
            Rotator.Draw(tf);

            tf.Rotation = {0, 0, 0};
            glUniform4f(ObjColPos, 0, 0.5, 0, 1);
            if(HoveredObjId == 2)
                glUniform4f(ObjColPos, 0, 1, 0, 1);
            glUniform1ui(ObjIdPos, 2);
            Rotator.Draw(tf);

            tf.Rotation = {90, 0, 0};
            glUniform4f(ObjColPos, 0, 0, 0.5, 1);
            if(HoveredObjId == 3)
                glUniform4f(ObjColPos, 0, 0, 1, 1);
            glUniform1ui(ObjIdPos, 3);
            Rotator.Draw(tf);

            glUniform4f(ObjColPos, 1, 1, 1, 1);
        }


    }

    glDisable(GL_DEPTH_TEST);
}

template<typename t>
inline float GetVecComponent(const t &v1, const t &v2) {
    auto NormComp = glm::normalize(v2);
    return (glm::dot(v1, NormComp));
}

template<typename t>
inline t ProjectToRayNormalized(const t &v1, const t &ray) {
    return glm::dot(v1, ray) / glm::dot(ray,ray) * ray;
}

template<typename t>
inline t ProjectToRay(const t &v1,const t &rayStart, const t &ray) {
    return ProjectToRayNormalized(v1-rayStart,ray - rayStart) + rayStart;
}

template<typename t>
inline t ProjectToRayNormDir(const t &v1,const t &rayStart, const t &RayDir) {
    return ProjectToRayNormalized(v1-rayStart,RayDir) + rayStart;
}

glm::vec3 MainViewport::ScreenSpaceToRay(glm::vec2 ScreenPos){
    auto InverseVP = glm::inverse(VP);

    glm::vec4 worldPos = InverseVP * glm::vec4(ScreenPos,1.0,1.0);

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}

void MainViewport::HandleInput(InputEvent event) {
    static glm::vec2 lastMpos = glm::vec2(0, 0);

    glm::vec2 currentMpos = glm::vec2(event.x, event.y);

    //static glm::vec3 GizmoDirPrePos = {0,0,0};

    glm::vec2 DeltaMousePos = currentMpos - lastMpos;

    static glm::vec3 ObjBindingVec = {0,0,0};
    static float ObjScalingDist = 0.0f;

    static glm::vec3 GizmoDir = glm::vec3(1, 1, 1);
    static unsigned int SpecialObjCur = 0;

    auto SelObj = GetMainWindow()->selectedElem;

    switch (event.EventType) {
        case (InputType::MouseHover):{
            GLint y = event.y * frameheight;
            GLint x = event.x * framewidth;

            glReadBuffer(GL_COLOR_ATTACHMENT1);
            glReadPixels(x,y,1,1, GL_RED_INTEGER, GL_UNSIGNED_INT, &HoveredObjId);
        }
        break;
        case (InputType::MouseDownL): {
            GLint y = event.y * frameheight;
            GLint x = event.x * framewidth;

            glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            unsigned int obj = 0;

            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &obj);


            if (obj < 16) {
                SpecialObjCur = obj;
                if (CurrentGizmoType == Move) {
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

                        glm::vec3 GizmoDirPrePos = ProjectToRayNormalized({CurWDirPos.x,CurWDirPos.y,CurWDirPos.z},GizmoDir);
                        glm::vec3 objpos = {SelObj->TF.Position.X,SelObj->TF.Position.Y,SelObj->TF.Position.Z};

                        ObjBindingVec = objpos - GizmoDirPrePos;

                        std::cout << glm::to_string(ObjBindingVec) << std::endl;
                    }
                } else if(CurrentGizmoType == Scale){
                    if(0 < obj && obj <= 3 ) {
                        if (obj == 1) {
                            GizmoDir = {1, 0, 0};
                        } else if (obj == 2) {
                            GizmoDir = {0, 1, 0};
                        } else if (obj == 3) {
                            GizmoDir = {0, 0, 1};
                        }

                        auto TF = SelObj->TF;

                        TF.Scale = {1,1,1};
                        TF.Position = {0,0,0};

                        auto mat = TransformToMatrix(TF);

                        GizmoDir = mat * glm::vec4(GizmoDir,1);

                        glReadBuffer(GL_COLOR_ATTACHMENT2);
                        glm::vec4 CurWDirPos = {0,0,0,0};

                         glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(CurWDirPos));

                        glm::vec3 GizmoDirPrePos = ProjectToRayNormalized({CurWDirPos.x,CurWDirPos.y,CurWDirPos.z},GizmoDir);

                        glm::vec3 objpos = ToGlmVec3(SelObj->TF.Position);

                        ObjScalingDist = glm::distance(ProjectToRayNormalized(objpos,GizmoDir), GizmoDirPrePos);

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

            GetMainWindow()->selectedElem = &GetMainWindow()->loadedMap.Objects[obj];
        }
            break;
        case (InputType::MouseHoldL): {
            GLint y = event.y * frameheight;
            GLint x = event.x * framewidth;

            glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            unsigned int obj = 0;

            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &obj);

            if (CurrentGizmoType == Move) {
                if(SpecialObjCur == 0) {
                    return;
                }
                /*if(SpecialObjCur != obj){
                    SpecialObjCur = 0;
                    return;
                }*/

                auto tf = SelObj->TF;

                tf.Rotation = {0,0,0};
                tf.Scale = {1,1,1};

                auto Tf = TransformToMatrix(tf);

                if (SpecialObjCur == 1) {
                    GizmoDir = {1, 0, 0};
                } else if (SpecialObjCur == 2) {
                    GizmoDir = {0, 1, 0};
                } else if (SpecialObjCur == 3) {
                    GizmoDir = {0, 0, 1};
                }

                auto ClipSpaceStart = (VP * Tf) * glm::vec4(0,0,0,1);
                auto ClipSpaceEnd = (VP * Tf) * glm::vec4(GizmoDir,1);

                glm::vec2 ScreenSpaceStart = {((ClipSpaceStart.x/ClipSpaceStart.w + 1 ) / 2) * framewidth, ((ClipSpaceStart.y/ClipSpaceStart.w + 1 ) / 2) * frameheight };
                glm::vec2 ScreenSpaceEnd = {((ClipSpaceEnd.x/ClipSpaceEnd.w + 1 ) / 2) * framewidth, ((ClipSpaceEnd.y/ClipSpaceEnd.w + 1 ) / 2) * frameheight};

                glm::vec2 ScrenSpaceDir = ScreenSpaceEnd - ScreenSpaceStart;

                glm::vec2 MousePos = {x,y};

                glm::vec2 FixedMousePos = ProjectToRayNormDir(MousePos,ScreenSpaceStart,ScrenSpaceDir);

                //SpecialObjCur = obj;
                if(0 < SpecialObjCur && SpecialObjCur <= 3 ) {
                    glReadBuffer(GL_COLOR_ATTACHMENT2);
                    glm::vec4 CurWDirPos = {0,0,0,0};

                    glReadPixels(floorf(FixedMousePos.x), floorf(FixedMousePos.y), 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(CurWDirPos));

                    //std::cout << glm::to_string(CurWDirPos) << std::endl;

                    auto DirProjectedTranslationPos = ProjectToRayNormalized({CurWDirPos.x,-CurWDirPos.y,CurWDirPos.z},GizmoDir);

                    if (SpecialObjCur == 1) {
                        SelObj->TF.Position.X = ObjBindingVec.x + DirProjectedTranslationPos.x;
                    } else if (SpecialObjCur == 2) {
                        SelObj->TF.Position.Y = ObjBindingVec.y + DirProjectedTranslationPos.y;
                    } else if (SpecialObjCur == 3) {
                        SelObj->TF.Position.Z = ObjBindingVec.z + DirProjectedTranslationPos.z;
                    }

                    glm::vec3 objpos = {SelObj->TF.Position.X,SelObj->TF.Position.Y,SelObj->TF.Position.Z};

                    ObjBindingVec = objpos - DirProjectedTranslationPos;
                    lastMpos = currentMpos;
                }
            } else if (CurrentGizmoType == Scale) {
                if(SpecialObjCur == 0)
                    return;
                if(SpecialObjCur != obj){
                    SpecialObjCur = 0;
                    return;
                }
                if(0 < SpecialObjCur && SpecialObjCur <= 3 ) {
                    SpecialObjCur = obj;
                    if (obj == 1) {
                        GizmoDir = {1, 0, 0};
                    } else if (obj == 2) {
                        GizmoDir = {0, 1, 0};
                    } else if (obj == 3) {
                        GizmoDir = {0, 0, 1};
                    }

                    auto TF = SelObj->TF;

                    TF.Scale = {1,1,1};
                    TF.Position = {0,0,0};

                    auto mat = TransformToMatrix(TF);

                    GizmoDir = mat * glm::vec4(GizmoDir,1);

                    glReadBuffer(GL_COLOR_ATTACHMENT2);
                    glm::vec4 CurWDirPos = {0,0,0,0};

                    //GizmoDir = glm::normalize( * GizmoDir)
                    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, glm::value_ptr(CurWDirPos));

                    glm::vec3 ObjPos = ToGlmVec3(SelObj->TF.Position);

                    ObjPos.y = -ObjPos.y;

                    CurWDirPos -= glm::vec4(ObjPos,1);



                    auto DirProjectedTranslationPos = ProjectToRayNormalized({CurWDirPos.x,-CurWDirPos.y,CurWDirPos.z},GizmoDir);

                    float VecComp = GetVecComponent(DirProjectedTranslationPos,GizmoDir);

                    float DeltaInDir = 0;

                    if (SpecialObjCur == 1) {
                        DeltaInDir = (ObjBindingVec.x + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.x) - SelObj->TF.Scale.X;
                        SelObj->TF.Scale.X = ObjBindingVec.x + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.x;
                    } else if (SpecialObjCur == 2) {
                        DeltaInDir = (ObjBindingVec.y + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.y) - SelObj->TF.Scale.Y;
                        SelObj->TF.Scale.Y = ObjBindingVec.y + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.y;
                    } else if (SpecialObjCur == 3) {
                        DeltaInDir = (ObjBindingVec.z + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.z) - SelObj->TF.Scale.Z;
                        SelObj->TF.Scale.Z = ObjBindingVec.z + ((VecComp / ObjScalingDist) - 1) * ObjBindingVec.z;
                    }

                    if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl)){
                        if(SpecialObjCur != 1) SelObj->TF.Scale.X += DeltaInDir;
                        if(SpecialObjCur != 2) SelObj->TF.Scale.Y += DeltaInDir;
                        if(SpecialObjCur != 3) SelObj->TF.Scale.Z += DeltaInDir;
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
            camrot.x += (lastMpos.x - currentMpos.x) * (float) framewidth * .1f;
            camrot.y -= (lastMpos.y - currentMpos.y) * (float) frameheight * .1f;
            lastMpos = currentMpos;
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

    auto SelObj = GetMainWindow()->selectedElem;


    auto DirPos = glm::vec4(dir, 1);
    auto ObjPos = glm::vec4(0, 0, 0, 1);


    auto tf = SelObj->TF;

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

MainViewport *GetMainViewport() {
    return MainVP;
}