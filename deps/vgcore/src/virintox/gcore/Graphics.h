#ifndef PNS_GRAPHICS_H
#define PNS_GRAPHICS_H
#include<string>
#include<array>
#include<GL/glew.h>
#include"GLFW/glfw3.h"
#include"gui/Window.h"

namespace Graphics {

    class InitAction : public VMirror::Reflectable {
        VABSTRACTCLASS(InitAction, ESC({nullptr}))
        InitAction() = default;
    };

    void AutoInit(std::string name);

    void Init();

    void BeginLoop();

    void Terminate();

    void EnableVsync();

    inline float MWheel;

    inline std::unique_ptr<Window> window;
    inline bool Running;

    template<class T>
    inline T *setWindow() {
        T *_window = new T();
        window = std::unique_ptr<Window>(_window);
        return _window;
    }

    inline void Quit() {
        Running = false;


#define VGINIT_ACTION(name) \
class name##InitAction: public Graphics::InitAction{ \
    VCLASS(name##InitAction,ESC({&Graphics::InitAction::classInfo,nullptr})) \
    name##InitAction() {     \
        Exec();                    \
        delete this;            \
    }\
    void Exec();\
};                          \
REGISTERVCLASS(name##InitAction)\
void name##InitAction::Exec()

    }
}
#endif