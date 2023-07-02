#ifndef PNS_OUTLINER_H
#define PNS_OUTLINER_H

#include<memory>
#include<virintox/gcore/Widget.h>
#include<cstring>
#include<virintox/gcore/ITreeSerializable.h>
namespace Graphics {
    class NameEditorOld : public Widget{
    public:
        NameEditorOld();

        char NewName[256];
        void PreDraw() override;
        void Draw() override;
        inline void Activate(ITreeSerializable* Node){
            memset(NewName,0,256);
            EditingNode = Node;
            Active = true;
        }

        ITreeSerializable* EditingNode;
    };

    class Outliner : public Widget{
    public:
        Outliner();
        void Draw() override;
        void DrawNode(ITreeSerializable* Node);
        inline void SetMainItem(ITreeSerializable* Item){
            LastSelectedNode = nullptr;
            MainItem = Item;
        }
        ITreeSerializable* MainItem;
        ITreeSerializable* LastSelectedNode;

        NameEditorOld NameEdit;

        void PostDraw() override;
    };

}

#endif //PNS_OUTLINER_H
