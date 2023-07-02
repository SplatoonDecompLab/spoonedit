#ifndef PNS_ITREESERIALIZABLE_H
#define PNS_ITREESERIALIZABLE_H

#include<string>
#include<vector>

enum ObjectType{
    Folder,
    Item
};

class ITreeSerializable{
public:
    virtual void SetParent(ITreeSerializable*) = 0;
    virtual std::string GetName() = 0;
    virtual void SetName(std::string) = 0;
    virtual void Release(ITreeSerializable*) = 0;
    virtual void Attach(ITreeSerializable*) = 0;

    virtual std::vector<ITreeSerializable*> GetSubItems() = 0;
    virtual ObjectType GetObjectType() = 0;

    bool CurrentlySelected;
};

#endif //PNS_ITREESERIALIZABLE_H
