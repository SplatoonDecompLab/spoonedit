#ifndef PNS_WIDGET_H
#define PNS_WIDGET_H
#include<string>
#include<vector>
#include<memory>
#include<GL/glew.h>
#include "WidgetElement.h"

namespace Graphics {
    class Widget {
    public:
        Widget(std::string name, bool active = false);
        virtual void Update();
        virtual void PreDraw();
        virtual void Draw();
        virtual void PostDraw();
        inline std::string GetName(){
            return Name;
        }
        bool Active;
    protected:
        std::vector<std::unique_ptr<WidgetElement>> Elements;
        std::string Name;
    public:
        virtual ~Widget() = default;
    };
}
#endif //PNS_WIDGET_H
