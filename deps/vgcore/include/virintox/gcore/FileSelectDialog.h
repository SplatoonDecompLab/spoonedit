
#ifndef PNS_FILESELECTDIALOG_H
#define PNS_FILESELECTDIALOG_H

#include<virintox/gcore/Widget.h>
#include<functional>
#include<boost/filesystem.hpp>



namespace Graphics{
    enum FileSelectMode{
        Open,
        Save
    };
    class FileSelectDialog : public Widget{
    public:
        FileSelectDialog(std::string name,std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenCloseFunc);
        FileSelectDialog(std::string name,std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenFunc, std::function<void(boost::filesystem::path)> OnSaveFunc);

        FileSelectDialog(std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenCloseFunc);
        FileSelectDialog(std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenFunc, std::function<void(boost::filesystem::path)> OnSaveFunc);
        void Draw() override;
        std::vector<std::string> Path;
        std::vector<std::string> Extension;
        boost::filesystem::path GetPath();

        std::string Filter;

        char Filename[256] = "";

        FileSelectMode fileSelectMode;

        ~FileSelectDialog() override;
    protected:
        int SelectedFile;
        std::function<void(boost::filesystem::path)> OnOpen;
        std::function<void(boost::filesystem::path)> OnSave;
    };
}
#endif //PNS_FILESELECTDIALOG_H
