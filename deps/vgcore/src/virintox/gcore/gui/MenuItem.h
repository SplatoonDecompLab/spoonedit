//
// Created by tv on 28.08.23.
//

#pragma once
#include<ReflectableClass.h>

namespace Graphics {

    class MenuItem: public VMirror::Reflectable {
        VABSTRACTCLASS(MenuItem,ESC({nullptr}))
        explicit MenuItem(std::string menu);

        virtual void draw() = 0;
        std::string getMenu();
    private:
        std::string m_menu;
    };

    class DefaultMenuItem: public MenuItem{
        VABSTRACTCLASS(DefaultMenuItem,ESC({&MenuItem::classInfo,nullptr}))
        explicit DefaultMenuItem(std::string name, const std::string &menu);

        void draw() override;

        virtual void onClick() = 0;
    private:
        std::string m_name;
    };



#define MENU_ITEM(menu,itemname) \
class MenuItem##menu##itemname: public Graphics::DefaultMenuItem{ \
    VCLASS(MenuItem##menu##itemname,ESC({&Graphics::DefaultMenuItem::classInfo,nullptr}))\
    MenuItem##menu##itemname(): Graphics::DefaultMenuItem(#itemname,#menu){} \
    void onClick() override;\
};                               \
REGISTERVCLASS(MenuItem##menu##itemname)\
void MenuItem##menu##itemname::onClick()

#define MENU_ITEM_DISPLAY(menu,itemname,displayText) \
class MenuItem##menu##itemname: public Graphics::DefaultMenuItem{ \
    VCLASS(MenuItem##menu##itemname,ESC({&Graphics::DefaultMenuItem::classInfo,nullptr}))\
    MenuItem##menu##itemname(): Graphics::DefaultMenuItem(displayText,#menu){} \
    void onClick() override;\
};                               \
REGISTERVCLASS(MenuItem##menu##itemname)\
void MenuItem##menu##itemname::onClick()


} // Graphics
