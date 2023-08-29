#include "virintox/gcore/gui/ViewportWidget.h"
#include "virintox/gcore/gui/Widget.h"
#include<DemoViewport.h>
#include<virintox/gcore/Texture.h>
#include<iostream>

Graphics::Texture img;

DemoViewport::DemoViewport() : Graphics::ViewportWidget("test", true) {
    img = Graphics::Texture("/home/tv/Pictures/aa.png");
}

void DemoViewport::Draw(){
    img.DrawC(glm::vec2(0,0),1);
    ChangeView(0,0,1);
}