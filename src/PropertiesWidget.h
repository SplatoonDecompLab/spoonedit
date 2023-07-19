//
// Created by tv on 21.05.23.
//

#ifndef SPOONTOOL_PROPERTIESWIDGET_H
#define SPOONTOOL_PROPERTIESWIDGET_H


#include "virintox/gcore/gui/Widget.h"

class PropertiesWidget : public Graphics::Widget {
public:
    PropertiesWidget();

    void Draw() override;
};

PropertiesWidget* GetPropertiesWidget();

#endif //SPOONTOOL_PROPERTIESWIDGET_H
