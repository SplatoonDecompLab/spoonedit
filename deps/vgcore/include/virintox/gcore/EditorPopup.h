#pragma once
#include <cstring>
#include<functional>

#include<virintox/gcore/Widget.h>

namespace Graphics{
    class NameEditor : public Widget{
    public:
        NameEditor(std::function<void(std::string)> func,std::string name = "Name Editor");

        char NewName[256];
        void PreDraw() override;
        void Draw() override;
        inline void Activate(const std::string &current){
            strcpy(NewName, current.c_str());
            Active = true;
        }

        std::function<void(std::string)> SetFunc;
    };
}