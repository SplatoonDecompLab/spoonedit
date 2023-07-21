//
// Created by tv on 21.05.23.
//

#ifndef SPOONTOOL_MAINVIEWPORT_H
#define SPOONTOOL_MAINVIEWPORT_H


#include <queue>
#include "virintox/gcore/gui/ViewportWidget.h"
#include "virintox/gcore/Shader.h"
#include "glm/vec3.hpp"
#include "virintox/gcore/Mesh.h"
#include "Model.h"
#include "boost/thread.hpp"
#include "Rail.h"
#include "LevelObject.h"

enum GizmoType{
    Move,Rotate,Scale
};

class MainViewport : public Graphics::ViewportWidget {
public:
    MainViewport();

    glm::vec3 camPos = glm::vec3(0,0,0);
    glm::vec2 camrot = glm::vec2(0,0);

    void clearModels();

    void cleanUnnescessary();
private:

    float shadowArea = 750.0f;

    Graphics::Shader flatShader;
    Graphics::Shader defaultShader;

    bool flatShading = false;


    float fov = 90.0f;

    bool greenScreen = false;

    //std::vector<std::vector<unsigned int>> PixelToObjNum;

    float speed = 100.0f;
    bool drawAllAreas = false;

    GizmoType CurrentGizmoType = Move;



    unsigned int HoveredObjId = 0;

    std::map<std::string,Model> MdlFromObj;

    float FarClippingPlane = 10000.0f;
    float NearClippingPlane = 0.1f;

    glm::mat4 VP;

    glm::vec3 LightDir = {1,1,1};

    float ShadingEffectiveness = 0.5f;

    void RenderRail(Rail* rail);

    //Graphics::Mesh ObjMesh;
    glm::vec2 CalcGizmoDir(glm::vec3 dir);
    glm::vec3 ScreenSpaceToRay(glm::vec2);
    void DrawOver() override;

    std::unique_ptr<Graphics::Framebuffer> SunCam;
public:
    void HandleInput(InputEvent event) override;

private:
    void Draw() override;
};


MainViewport* g_getMainViewport();

#endif //SPOONTOOL_MAINVIEWPORT_H
