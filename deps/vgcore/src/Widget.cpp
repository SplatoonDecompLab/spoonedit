#include<virintox/gcore/Graphics.h>
#include<virintox/gcore/Widget.h>
#include<imgui.h>
namespace Graphics{

    Widget::Widget(std::string name, bool active) {
        Name = name;
        Active = active;
        Elements = std::vector<std::unique_ptr<WidgetElement>>();
    }

    void Widget::Update() {

    }

    void Widget::PreDraw() {
        ImGui::Begin(Name.c_str(),&Active);
    }
    void Widget::Draw() {

    }

    void Widget::PostDraw() {
        ImGui::End();
    }


}