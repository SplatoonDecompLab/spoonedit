#ifndef PNS_WINDOW_H
#define PNS_WINDOW_H
#include "MessageBox.h"
#include "Widget.h"

#include"GLFW/glfw3.h"
#include<memory>
#include<string>
#include <utility>
#include<vector>
#include<functional>
#include<map>
#include<list>
#include"MenuItem.h"

namespace Graphics{
    void AutoInit(std::string name);
    class InitAction;

    struct Menu{
        inline explicit Menu(std::string name): m_name(std::move(name)){}
        std::string m_name;
        unsigned int m_sortId = 0;
        std::vector<std::unique_ptr<MenuItem>> m_items = std::vector<std::unique_ptr<MenuItem>>();
    };

    class MenuSortSetter: public VMirror::Reflectable{
        VABSTRACTCLASS(MenuSortSetter,ESC({nullptr}))

    public:
        MenuSortSetter(std::string menuName,unsigned int id);
    };

    class Window{
        friend class MenuSortSetter;
        friend class InitAction;

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

        void addTemporaryWidget(Widget* widget){
            for(Widget* tmpWidget: TemporaryWidgets){
                if(widget->GetName() == tmpWidget->GetName()){
                    delete widget;
                    return;
                }
            }
            TemporaryWidgets.push_back(widget);
        }

        std::list<Widget*> TemporaryWidgets;


        virtual void DropCallback(const std::vector<std::string> &paths);

        std::list<MessageBox*> msgBoxes;

        friend void Graphics::AutoInit(std::string name);

        template <class T>
        inline T* addWidget(T* t){

            Widgets.push_back(std::unique_ptr<Widget>(t));
            WidgetByName.insert({t->GetName(),t});
            return t;
        };

    protected:

        std::vector<Menu> Menus;

        inline void addMenu(const std::string &Title){
            Menus.emplace_back(Title);
        }

        inline void addMenuItem(MenuItem *item){
            auto iter = std::find_if(Menus.begin(), Menus.end(),[&](const Menu& menu){
                return menu.m_name == item->getMenu();
            });
            if(iter == Menus.end()){
                addMenu(item->getMenu());
                iter = std::find_if(Menus.begin(), Menus.end(),[&](const Menu& menu){
                    return menu.m_name == item->getMenu();
                });
            }

            iter->m_items.push_back(std::unique_ptr<MenuItem>(item));
        }

        template <class T>
        inline T* addWidget(){
            T* t = new T();
            Widgets.push_back(std::unique_ptr<Widget>(t));
            WidgetByName.insert({t->GetName(),t});
            return t;
        };

        GLFWwindow* window;
    };




#define SORT_MENU(MenuName,id) \
class MenuName##SortSetter: public Graphics::MenuSortSetter{ \
    VCLASS(MenuName##SortSetteer, ESC({&Graphics::MenuSortSetter::classInfo,nullptr}))\
    MenuName##SortSetter(): Graphics::MenuSortSetter(#MenuName,id){                   \
        delete this;               \
    } \
};                              \
REGISTERVCLASS(MenuName##SortSetter)

}

#endif //PNS_WINDOW_H
