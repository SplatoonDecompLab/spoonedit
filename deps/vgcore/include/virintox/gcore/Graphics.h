#ifndef PNS_GRAPHICS_H
#define PNS_GRAPHICS_H
#include<string>
#include<array>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<virintox/gcore/Window.h>

namespace Graphics{

    void Init();
    void BeginLoop();
    void Terminate();

    void EnableVsync();

    inline float MWheel;

    inline std::unique_ptr<Window> window;
    inline bool Running;

    template<class T>
    inline T* setWindow(){
        T* _window = new T();
        window = std::unique_ptr<Window>(_window);
        return _window;
    }

    inline void Quit(){
        Running = false;
    }


}
#endif