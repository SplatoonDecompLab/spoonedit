//
// Created by tv on 28.06.23.
//

#ifndef SPOONTOOL_RAILSELECTWIDGET_H
#define SPOONTOOL_RAILSELECTWIDGET_H

#include<virintox/gcore/gui/Widget.h>


class RailSelectWidget: public Graphics::Widget {
public:
    VCLASS(RailSelectWidget,ESC({&Graphics::Widget::classInfo,nullptr}))
    RailSelectWidget();

    void Draw() override;
};


#endif //SPOONTOOL_RAILSELECTWIDGET_H
