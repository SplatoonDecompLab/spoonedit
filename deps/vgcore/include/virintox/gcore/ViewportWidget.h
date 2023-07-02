#ifndef PNS_VIEWPORTWIDGET_H
#define PNS_VIEWPORTWIDGET_H
#include"Viewport.h"
#include"Widget.h"
namespace Graphics{
    class ViewportWidget :  public Graphics::Viewport , public Graphics::Widget{
    public:
        ViewportWidget(std::string name,bool active);
        void PreDraw() override;
        void PostDraw() override;
        virtual void DrawOver();
    };
}
#endif