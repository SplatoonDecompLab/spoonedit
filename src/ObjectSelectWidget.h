//
// Created by tv on 21.05.23.
//

#ifndef SPOONTOOL_OBJECTSELECTWIDGET_H
#define SPOONTOOL_OBJECTSELECTWIDGET_H


#include "virintox/gcore/gui/Widget.h"

class ObjectSelectWidget: public Graphics::Widget {
public:
    VCLASS(ObjectSelectWidget,ESC({&Graphics::Widget::classInfo,nullptr}))
    ObjectSelectWidget();

private:
    void Draw() override;
};

ObjectSelectWidget* GetObjectSelectWidget();

#endif //SPOONTOOL_OBJECTSELECTWIDGET_H
