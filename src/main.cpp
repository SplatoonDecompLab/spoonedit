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

enum class FileMode{
    Compiled,
    Uncompiled
};


static FileMode g_fileMode = FileMode::Uncompiled;

VGINIT_ACTION(AddMapSelector){
    Graphics::FileSelectDialog* fsdiag = new Graphics::FileSelectDialog("Select/Save Map", {".yaml"},[&](boost::filesystem::path path){
        selectedElem = nullptr;

        if(g_fileMode == FileMode::Compiled)
            loadedMap = ConvertFromYaml(path);
        if(g_fileMode == FileMode::Uncompiled)
            loadedMap = Map(path);

        MainViewport* vp = g_getMainViewport();
        vp->cleanUnnescessary();
    },[&](boost::filesystem::path path){
        if(g_fileMode == FileMode::Compiled)
            loadedMap.Export(path);
        if(g_fileMode == FileMode::Uncompiled)
            loadedMap.Save(path);
    });

    Graphics::window->addWidget(fsdiag);
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

    g_fileMode = FileMode::Uncompiled;

    std::vector<std::string> strvec;
    boost::algorithm::split(strvec,(boost::filesystem::current_path() / "Maps").string(),boost::algorithm::is_any_of("/\\"));

    wid->Path = strvec;

    wid->fileSelectMode = Graphics::Open;
    wid->Active = true;
}

MENU_ITEM_DISPLAY(File,Save,ICON_FA_FLOPPY_DISK " Save"){
    auto wid = dynamic_cast<Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Select/Save Map")->second);

    g_fileMode = FileMode::Uncompiled;

    std::vector<std::string> strvec;
    boost::algorithm::split(strvec,(boost::filesystem::current_path() / "Maps").string(),boost::algorithm::is_any_of("/\\"));

    wid->Path = strvec;

    wid->fileSelectMode = Graphics::Save;
    wid->Active = true;
}

MENU_ITEM_DISPLAY(File,Import,ICON_FA_ARROW_LEFT " Import"){
    auto wid = dynamic_cast<Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Select/Save Map")->second);

    g_fileMode = FileMode::Compiled;

    std::vector<std::string> strvec;
    boost::algorithm::split(strvec,(boost::filesystem::current_path() / "CompiledMaps").string(),boost::algorithm::is_any_of("/\\"));

    wid->Path = strvec;

    wid->fileSelectMode = Graphics::Open;
    wid->Active = true;
}

MENU_ITEM_DISPLAY(File,Export,ICON_FA_ARROW_RIGHT " Export"){
    auto wid = dynamic_cast<Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Select/Save Map")->second);

    g_fileMode = FileMode::Compiled;

    std::vector<std::string> strvec;
    boost::algorithm::split(strvec,(boost::filesystem::current_path() / "CompiledMaps").string(),boost::algorithm::is_any_of("/\\"));

    wid->Path = strvec;

    wid->fileSelectMode = Graphics::Save;
    wid->Active = true;
}



SORT_MENU(File,0)
SORT_MENU(Widgets,1)
SORT_MENU(Object,2)

int main(){
    Graphics::AutoInit("SpoonEdit");
}