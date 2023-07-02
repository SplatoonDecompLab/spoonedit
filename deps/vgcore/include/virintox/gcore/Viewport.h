#ifndef PNS_VIEWPORT_H
#define PNS_VIEWPORT_H
#include<GL/glew.h>
#include<glm/vec2.hpp>
#include"Widget.h"

enum InputType{
    MouseDownR,
    MouseDownL,
    MouseUpR,
    MouseUpL,
    MouseHoldR,
    MouseHoldL,
    MouseHover,
    MWheel
};

struct InputEvent{
    float x;
    float y;
    InputType EventType;
    float MWheel;
};

namespace Graphics {
    class Viewport{
    public:
        Viewport();
        unsigned int Framebuffer;
        unsigned int FramebufferTex;
        unsigned int FramebufferDepthTex;
        unsigned int FramebufferMatIdTex;
        unsigned int FramebufferWorldPosTex;

        void ChangeView(const glm::vec2 &pos,float scale) const;
        void ChangeView(float x,float y, float scale) const;

        virtual void Draw() = 0;
        virtual void HandleInput(InputEvent);


    protected:
        void RecreateFramebuffer();

        int frameheight,framewidth;
        int lastframeheight,lastframewidth;
    };
}
#endif //PNS_VIEWPORT_H
