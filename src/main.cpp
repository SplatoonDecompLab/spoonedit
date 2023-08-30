#include<virintox/gcore/Graphics.h>
#include"imgui.h"
#include"virintox/gcore/gui/FileSelectDialog.h"
#include"InstanceVars.h"
#include"Map.h"
#include"MainViewport.h"
#include"Config/Configs.h"
#include<IconsFontAwesome6.h>
#include<Assembly.h>

VMIRRORASSEMBLYSELFREGISTER(SpoonEdit, VMIRRORVERSION(0,1,0,0),VMirror::AssemblyType::Executable)

VGINIT_ACTION(AddMapSelector){
    Graphics::window->addWidget(new Graphics::FileSelectDialog("Select/Save Map", {".yaml"},[&](boost::filesystem::path path){
        selectedElem = nullptr;
        loadedMap = ConvertFromYaml(path);

        MainViewport* vp = g_getMainViewport();
        vp->cleanUnnescessary();
    },[&](boost::filesystem::path path){
        loadedMap.Export(path);
    }));
}

VGINIT_ACTION(AddObjSelector){
    Graphics::window->addWidget(new Graphics::FileSelectDialog("Load Object from Preset", {".yaml"},[](boost::filesystem::path path){
        auto file = YAML::LoadFile(path.string());
        selectedElem = nullptr;
        loadedMap.Objects.emplace_back(LevelObject(file));
    }));
}

MENU_ITEM_DISPLAY(File,Open,ICON_FA_FOLDER_OPEN " Open"){
    auto wid = dynamic_cast<Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Select/Save Map")->second);
    wid->fileSelectMode = Graphics::Open;
    wid->Active = true;
}

MENU_ITEM_DISPLAY(File,Save,ICON_FA_FLOPPY_DISK " Save"){
    auto wid = dynamic_cast<Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Select/Save Map")->second);
    wid->fileSelectMode = Graphics::Save;
    wid->Active = true;
}

SORT_MENU(File,0)
SORT_MENU(Widgets,1)
SORT_MENU(Object,2)

int main(){
    Graphics::AutoInit("SpoonEdit");
}