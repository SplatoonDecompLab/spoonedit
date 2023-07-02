#ifndef PNS_WIDGETELEMENT_H
#define PNS_WIDGETELEMENT_H

namespace Graphics {
    class WidgetElement {
    public:
        virtual void Render() = 0;
        virtual ~WidgetElement() = default;

    };
}

#endif //PNS_WIDGETELEMENT_H
