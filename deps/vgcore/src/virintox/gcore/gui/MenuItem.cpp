#include"MenuItem.h"

#include <utility>
#include<imgui.h>

namespace Graphics{
    REGISTERABSTRACTVCLASS(MenuItem)

    MenuItem::MenuItem(std::string menu): m_menu(std::move(menu)) {}

    std::string MenuItem::getMenu() {
        return m_menu;
    }

    REGISTERABSTRACTVCLASS(DefaultMenuItem)

    DefaultMenuItem::DefaultMenuItem(std::string name, const std::string &menu): MenuItem(menu), m_name(std::move(name)) {}

    void DefaultMenuItem::draw() {
        if(ImGui::MenuItem(m_name.c_str()))
            onClick();
    }

}