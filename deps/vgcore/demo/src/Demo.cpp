#include"virintox/gcore/Graphics.h"
#include"Demo.h"
#include"virintox/gcore/Window.h"
#include"virintox/gcore/FileSelectDialog.h"
#include<DemoViewport.h>
#include "imgui.h"
#include"iostream"

Demo::Demo() : Graphics::Window("GCoreDemo"){
    THIS = this;

    addWidget<DemoViewport>();

    AddMenu("File");
    AddMenu("Widgets");

    AddMenuItem("File", "Open", [](){
        auto fsdiag = static_cast<Graphics::FileSelectDialog*>(THIS->WidgetByName.find("Select File")->second);
        
        fsdiag->fileSelectMode = Graphics::FileSelectMode::Open;

        fsdiag->Active = true;
    });

    AddMenuItem("File", "Quit", [](){
        Graphics::Quit();
    });

    AddMenuItem("Widgets", "Demo Viewport", [](){
        THIS->WidgetByName.find("test")->second->Active ^= 1;
    });
    


    addWidget<Graphics::FileSelectDialog>(new Graphics::FileSelectDialog(std::vector<std::string>{".txt"},[](boost::filesystem::path file){std::cout << file;}));
}

void Demo::Update(){
    
}