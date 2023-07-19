//
// Created by tv on 20.05.23.
//

#ifndef SPOONTOOL_MAINWINDOW_H
#define SPOONTOOL_MAINWINDOW_H

#include<virintox/gcore/gui/Window.h>
#include<Map.h>
namespace GameMode {
    enum GameMode {
        Gambit,
        Blitz,
        Thunder
    };

    inline std::string ToString(GameMode gameMode){
        switch(gameMode){
            case Gambit:
                return "Gambit";
            case Blitz:
                return "Blitz";
            case Thunder:
                return "Thunder";
            default:
                return "Invalid";
        }
    }

    inline std::array<GameMode,3> gameModes = {Gambit, Blitz, Thunder};
}


class MainWindow : public Graphics::Window {
public:
    MainWindow();

    Map loadedMap;

    void Update() override;

    Element* selectedElem = nullptr;

    GameMode::GameMode gameSetting = GameMode::Gambit;
protected:
    void MenuExtraRender() override;
};

MainWindow* GetMainWindow();


#endif //SPOONTOOL_MAINWINDOW_H
