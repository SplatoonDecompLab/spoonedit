#include<virintox/gcore/Graphics.h>
#include <IconsFontAwesome6.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "virintox/gcore/gui/FileSelectDialog.h"

MENU_ITEM_DISPLAY(Object,Add,ICON_FA_PLUS " Add"){
    auto wid = dynamic_cast<Graphics::FileSelectDialog*>(Graphics::window->WidgetByName.find("Load Object from Preset")->second);
    std::vector<std::string> strvec;
    boost::algorithm::split(strvec,(boost::filesystem::current_path() / "Presets").string(),boost::algorithm::is_any_of("/\\"));

    wid->Path = strvec;
    wid->fileSelectMode = Graphics::Open;
    wid->Active = true;
}