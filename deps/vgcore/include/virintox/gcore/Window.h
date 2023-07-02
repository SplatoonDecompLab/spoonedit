#ifndef PNS_WINDOW_H
#define PNS_WINDOW_H
#include "MessageBox.h"
#include "Widget.h"

#include<GLFW/glfw3.h>
#include<memory>
#include<string>
#include<vector>
#include<functional>
#include<map>
#include<list>

struct MenuItem{
    std::string Name;
    std::function<void(void)> Func;
    std::string Shortcut;
    int ShortcutKey;
};

namespace Graphics{
    class Window{
    public:
        explicit Window(const std::string &name = "Unnamed GCore Window");
        virtual void Update();

        virtual void DrawBG();
        void Draw();

        inline GLFWwindow* GetGlfwWindow(){
            return window;
        };

        std::vector<std::unique_ptr<Widget>> Widgets;
        std::map<std::string,Widget*> WidgetByName;

        virtual void DropCallback(const std::vector<std::string> &paths);

        std::list<MessageBox*> msgBoxes;

    protected:
        std::map<std::string,std::vector<MenuItem>> Menus;

        inline void AddMenu(std::string Title){
            Menus.insert({Title,std::vector<MenuItem>()});
        }

        inline void AddMenuItem(std::string Menu,std::string name,std::function<void()> func, std::string shortcut = "",int ShortcutKey = 0){
            Menus.find(Menu)->second.push_back(MenuItem{name,func,shortcut, ShortcutKey});
        }

        template <class T>
        inline T* addWidget(){
            T* t = new T();
            Widgets.push_back(std::unique_ptr<Widget>(t));
            WidgetByName.insert({t->GetName(),t});
            return t;
        };

        template <class T>
        inline T* addWidget(T* t){

            Widgets.push_back(std::unique_ptr<Widget>(t));
            WidgetByName.insert({t->GetName(),t});
            return t;
        };

        GLFWwindow* window;
    };
}

#endif //PNS_WINDOW_H
