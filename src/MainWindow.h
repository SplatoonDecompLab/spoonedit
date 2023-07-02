//
// Created by tv on 20.05.23.
//

#ifndef SPOONTOOL_MAINWINDOW_H
#define SPOONTOOL_MAINWINDOW_H

#include<virintox/gcore/Window.h>
#include<Map.h>


class MainWindow : public Graphics::Window {
public:
    MainWindow();

    Map loadedMap;

    void Update() override;

    Element* selectedElem = nullptr;
};

MainWindow* GetMainWindow();


#endif //SPOONTOOL_MAINWINDOW_H
