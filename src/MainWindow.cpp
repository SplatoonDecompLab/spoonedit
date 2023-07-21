#include "MainWindow.h"
#include "PropertiesWidget.h"
#include "ObjectSelectWidget.h"
#include "RailSelectWidget.h"
#include "MainViewport.h"
#include <virintox/gcore/gui/FileSelectDialog.h>
#include <virintox/gcore/Graphics.h>
#include<imgui.h>
#include <IconsFontAwesome6.h>
#include<fstream>
#include"Config/Configs.h"

MainWindow* MainWindowInst = nullptr;

void ExportObj(LevelObject obj){
    YAML::Emitter emitter;

    obj.YamlInsert(emitter);

    auto fstr = std::ofstream((boost::filesystem::current_path() / "Presets" / (obj.Name + ".yaml")).string());

    fstr << emitter.c_str();
    fstr.flush();
    fstr.close();
}

MainWindow::MainWindow(): Graphics::Window("SpoonEdit") {

    MainWindowInst = this;

    auto mapSelectSaveExplorer = addWidget(new Graphics::FileSelectDialog("Select/Save Map", {".yaml"},[&](boost::filesystem::path path){
        selectedElem = nullptr;
        GetMainWindow()->loadedMap = ConvertFromYaml(path);

        MainViewport* vp = g_getMainViewport();
        vp->cleanUnnescessary();
    },[&](boost::filesystem::path path){
        loadedMap.Export(path);
    }));

    std::vector<std::string> strvec;
    boost::algorithm::split(strvec,(boost::filesystem::current_path() / "Maps").string(),boost::is_any_of("/"));

    mapSelectSaveExplorer->Path = strvec;

    addWidget(new Graphics::FileSelectDialog("Load Object from Preset", {".yaml"},[](boost::filesystem::path path){
        auto file = YAML::LoadFile(path.string());
        MainWindowInst->selectedElem = nullptr;
        MainWindowInst->loadedMap.Objects.emplace_back(LevelObject(file));
    }));

    addWidget<PropertiesWidget>();

    addWidget<ObjectSelectWidget>();

    addWidget<MainViewport>();

    addWidget<RailSelectWidget>();

    AddMenu("File");

    AddMenuItem("File",ICON_FA_FOLDER_OPEN " Open",[](){
        auto wid = dynamic_cast<Graphics::FileSelectDialog*>(MainWindowInst->WidgetByName.find("Select/Save Map")->second);
        wid->fileSelectMode = Graphics::Open;
        wid->Active = true;
    });

    AddMenuItem("File",ICON_FA_FLOPPY_DISK " Save",[](){
        auto wid = dynamic_cast<Graphics::FileSelectDialog*>(MainWindowInst->WidgetByName.find("Select/Save Map")->second);
        wid->fileSelectMode = Graphics::Save;
        wid->Active = true;
    });

    AddMenu("Widgets");

    AddMenuItem("Widgets",ICON_FA_VIDEO " Viewport",[](){
        MainWindowInst->WidgetByName.find("Main Viewport")->second->Active ^= true;
    });

    AddMenuItem("Widgets",ICON_FA_CUBES " Objects",[](){
        MainWindowInst->WidgetByName.find("Object Select")->second->Active ^= true;
    });

    AddMenuItem("Widgets",ICON_FA_CIRCLE_NODES " Rails",[](){
        MainWindowInst->WidgetByName.find("Rails")->second->Active ^= true;
    });

    AddMenuItem("Widgets",ICON_FA_CLIPBOARD_LIST " Properties",[](){
        MainWindowInst->WidgetByName.find("Properties")->second->Active ^= true;
    });

    AddMenu("Game");
    AddMenuItem("Game", "Gambit(1)", [&](){
        gameSetting = GameMode::Gambit;
        Configs::g_loadConfigs(GameMode::ToString(gameSetting));
        g_getMainViewport()->clearModels();
    });
    AddMenuItem("Game", "Blitz(2)", [&](){
        gameSetting = GameMode::Blitz;
        Configs::g_loadConfigs(GameMode::ToString(gameSetting));
    });
    AddMenuItem("Game", "Thunder(3)", [&](){
        gameSetting = GameMode::Thunder;
        Configs::g_loadConfigs(GameMode::ToString(gameSetting));
    });

    AddMenu("Object");

    AddMenuItem("Object", ICON_FA_PLUS " Add",[](){
        auto wid = dynamic_cast<Graphics::FileSelectDialog*>(MainWindowInst->WidgetByName.find("Load Object from Preset")->second);
        std::vector<std::string> strvec;
        boost::algorithm::split(strvec,(boost::filesystem::current_path() / "Presets").string(),boost::algorithm::is_any_of("/\\"));

        wid->Path = strvec;
        wid->fileSelectMode = Graphics::Open;
        wid->Active = true;
    });

    AddMenuItem("Object", ICON_FA_ANCHOR " Kotzen[!]",[&](){
        for (LevelObject &obj: loadedMap.Objects) {
            if(!boost::filesystem::exists(boost::filesystem::current_path() / "Presets" / (obj.Type + ".yaml"))){
                LevelObject ObjCpy = LevelObject(obj);
                ObjCpy.Name = ObjCpy.Type;
                ObjCpy.Links = std::vector<Link>();
                ObjCpy.TF.Scale = {1, 1, 1};
                ObjCpy.TF.Rotation = {0, 0, 0};
                ObjCpy.TF.Position = {0, 0, 0};

                for(long &param: ObjCpy.Parameters){
                    param = -99;
                }

                for(float &param: ObjCpy.FloatParameters){
                    param = -99.0f;
                }

                ObjCpy.Layer = Common;

                ExportObj(ObjCpy);
            }
        }
    });

    AddMenuItem("Object", ICON_FA_ANCHOR " Durchfall[!]",[&](){

       for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(boost::filesystem::current_path() / "Maps"), {})) {

           std::cout << "Exporting all from " << entry << std::endl;

           MainWindowInst->selectedElem = nullptr;
           MainWindowInst->loadedMap = ConvertFromYaml(entry);

           for (LevelObject &obj: loadedMap.Objects) {
               if (!boost::filesystem::exists(boost::filesystem::current_path() / "Presets" / (obj.Type + ".yaml"))) {
                   LevelObject ObjCpy = LevelObject(obj);
                   ObjCpy.Name = ObjCpy.Type;
                   ObjCpy.Links = std::vector<Link>();

                   for(long &param: ObjCpy.Parameters){
                       param = -99;
                   }

                   for(float &param: ObjCpy.FloatParameters){
                       param = -99.0f;
                   }

                   ObjCpy.Layer = Common;

                   ObjCpy.TF.Scale = {1, 1, 1};
                   ObjCpy.TF.Rotation = {0, 0, 0};
                   ObjCpy.TF.Position = {0, 0, 0};

                   ExportObj(ObjCpy);
               }
           }
       }
    });

    msgBoxes.push_back(new Graphics::MessageBox("Gizmos are currently broken beware of using them!"));
    //msgBoxes.
    //Graphics::EnableVsync();

    Configs::g_loadConfigs("Gambit");

    glfwMakeContextCurrent(GetGlfwWindow());
    glfwSwapInterval(0);
}

void MainWindow::Update() {

}

void MainWindow::MenuExtraRender() {
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(("Game: " + GameMode::ToString(gameSetting) + " ").c_str()).x);
    ImGui::Text("%s", ("Game: " + GameMode::ToString(gameSetting)).c_str());
}

MainWindow* GetMainWindow(){
    return MainWindowInst;
}