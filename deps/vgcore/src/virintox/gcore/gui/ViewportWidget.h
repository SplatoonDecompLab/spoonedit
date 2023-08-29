#ifndef PNS_VIEWPORTWIDGET_H
#define PNS_VIEWPORTWIDGET_H
#include"../Framebuffer.h"
#include"Widget.h"
namespace Graphics{
    class ViewportWidget : public Graphics::Widget{
    public:
        VABSTRACTCLASS(ViewportWidget,ESC({&Widget::classInfo}))
        ViewportWidget(std::string name,bool active);
        void PreDraw() override;
        void PostDraw() override;
        virtual void DrawOver();
        virtual void HandleInput(InputEvent);

    protected:
        std::unique_ptr<Graphics::Framebuffer> m_internalFramebuf;
    };
}
#endif