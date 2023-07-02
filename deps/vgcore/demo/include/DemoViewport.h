#pragma once
#include<virintox/gcore/ViewportWidget.h>

class DemoViewport : public Graphics::ViewportWidget{
public:
    DemoViewport();
    void Draw() override;
    ~DemoViewport() = default;
};