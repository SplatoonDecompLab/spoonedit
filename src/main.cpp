#include<virintox/gcore/Graphics.h>
#include"MainWindow.h"
#include"imgui.h"
int main(){
    Graphics::Init();
    Graphics::setWindow<MainWindow>();

    //auto &io = ImGui::GetIO();

    //io.ConfigWindowsMoveFromTitleBarOnly = true;
    Graphics::BeginLoop();
    Graphics::Terminate();
}