#include<virintox/gcore/Graphics.h>
#include <IconsFontAwesome6.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "virintox/gcore/gui/FileSelectDialog.h"
#include "InstanceVars.h"
#include<fstream>

void ExportObj(LevelObject obj){
    YAML::Emitter emitter;

    obj.YamlInsert(emitter);

    auto fstr = std::ofstream((boost::filesystem::current_path() / "Presets" / (obj.Name + ".yaml")).string());

    fstr << emitter.c_str();
    fstr.flush();
    fstr.close();
}


MENU_ITEM_DISPLAY(Object,Add,ICON_FA_PLUS " Add"){
    auto wid = dynamic_cast<Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Load Object from Preset")->second);
    std::vector<std::string> strvec;
    boost::algorithm::split(strvec,(boost::filesystem::current_path() / "Presets").string(),boost::algorithm::is_any_of("/\\"));

    wid->Path = strvec;
    wid->fileSelectMode = Graphics::Open;
    wid->Active = true;
}

MENU_ITEM_DISPLAY(Object,Kotzen,"Kotzen[!]"){
    for (LevelObject &obj: loadedMap.Objects) {
        if(!boost::filesystem::exists(boost::filesystem::current_path() / "Presets" / (obj.Type + ".yaml"))){
            LevelObject ObjCpy = LevelObject(obj);
            ObjCpy.Name = ObjCpy.Type;
            ObjCpy.Links = std::vector<Link>();
            ObjCpy.TF.Scale = {1, 1, 1};
            ObjCpy.TF.Rotation = {0, 0, 0};
            ObjCpy.TF.Position = {0, 0, 0};

            for(int &param: ObjCpy.Parameters){
                param = -99;
            }

            for(float &param: ObjCpy.FloatParameters){
                param = -99.0f;
            }

            ObjCpy.Layer = Common;

            ExportObj(ObjCpy);
        }
    }
}

MENU_ITEM_DISPLAY(Object,Durchfall,"Durchfall[!]"){

    for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(boost::filesystem::current_path() / "Maps"), {})) {

        std::cout << "Exporting all from " << entry << std::endl;

        selectedElem = nullptr;
        loadedMap = ConvertFromYaml(entry);

        for (LevelObject &obj: loadedMap.Objects) {
            if (!boost::filesystem::exists(boost::filesystem::current_path() / "Presets" / (obj.Type + ".yaml"))) {
                LevelObject ObjCpy = LevelObject(obj);
                ObjCpy.Name = ObjCpy.Type;
                ObjCpy.Links = std::vector<Link>();

                for(int &param: ObjCpy.Parameters){
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
}



