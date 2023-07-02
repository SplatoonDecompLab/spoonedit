//
// Created by tv on 21.05.23.
//

#ifndef SPOONTOOL_MAINVIEWPORT_H
#define SPOONTOOL_MAINVIEWPORT_H


#include <queue>
#include "virintox/gcore/ViewportWidget.h"
#include "virintox/gcore/Shader.h"
#include "glm/vec3.hpp"
#include "virintox/gcore/Mesh.h"
#include "Model.h"
#include "boost/thread.hpp"

enum GizmoType{
    Move,Rotate,Scale
};

class MainViewport : public Graphics::ViewportWidget {
public:
    MainViewport();

    Graphics::Shader shader;

    glm::vec3 camPos = glm::vec3(0,0,0);
    glm::vec2 camrot = glm::vec2(0,0);

    float fov = 90.0f;

    std::vector<std::vector<unsigned int>> PixelToObjNum;

    float speed = 100.0f;
    bool drawAllAreas = false;

    GizmoType CurrentGizmoType = Move;

private:

    unsigned int HoveredObjId = 0;

    std::map<std::string,Model> MdlFromObj;

    glm::mat4 VP;

    //Graphics::Mesh ObjMesh;
    glm::vec2 CalcGizmoDir(glm::vec3 dir);
    glm::vec3 ScreenSpaceToRay(glm::vec2);
    void DrawOver() override;
public:
    void HandleInput(InputEvent event) override;

private:
    void Draw() override;
};


MainViewport* GetMainViewport();

#endif //SPOONTOOL_MAINVIEWPORT_H
