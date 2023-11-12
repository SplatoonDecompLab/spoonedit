//
// Created by tv on 07.10.23.
//
#include<virintox/gcore/Graphics.h>
#include "InstanceVars.h"

MENU_ITEM_DISPLAY(Mirror,FlipZ,"Flip Map"){
    std::list<LevelObject> toBeAdded;

    for(LevelObject lo: loadedMap.Objects){
        lo.Team = Teams::flipTeam(lo.Team);

        lo.TF.Position.Z = -lo.TF.Position.Z;
        lo.TF.Position.X = -lo.TF.Position.X;
        lo.TF.Rotation.Y = lo.TF.Rotation.Y + 180;

        lo.runtimeID = std::rand();

        toBeAdded.push_back(lo);
    }

    for(LevelObject &lo: toBeAdded){
        loadedMap.Objects.push_back(lo);
    }
}

MENU_ITEM_DISPLAY(Mirror,DelZ,"Delete +Z"){
    for(auto iter = loadedMap.Objects.begin(); iter != loadedMap.Objects.end(); iter++) {
        if(iter->TF.Position.Z > 0){
            auto itertemp = iter;
            iter--;
            loadedMap.Objects.erase(itertemp);
            selectedElem = nullptr;
        }
    }
}

SORT_MENU(Mirror,4)