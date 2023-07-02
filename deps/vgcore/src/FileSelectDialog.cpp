#include<virintox/gcore/FileSelectDialog.h>
#include <cstring>
#include<imgui.h>
#include <boost/algorithm/string.hpp>
#include "IconsFontAwesome6.h"
#include<misc/cpp/imgui_stdlib.h>
#ifdef WIN32
#include <windows.h>
#endif

namespace Graphics{

    FileSelectDialog::FileSelectDialog(std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenCloseFunc)\
    : FileSelectDialog("Select File",FileType, OnOpenCloseFunc) {}
    FileSelectDialog::FileSelectDialog(std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenFunc, std::function<void(boost::filesystem::path)> OnSaveFunc)\
    : FileSelectDialog("Select File",FileType, OnOpenFunc, OnSaveFunc) {}
    

    FileSelectDialog::FileSelectDialog(std::string name,std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenCloseFunc) : Widget(name, false) {
        Path = std::vector<std::string>();
        OnOpen = std::move(OnOpenCloseFunc);
        OnSave = std::move(OnOpenCloseFunc);
        Extension = FileType;

        SelectedFile = 0;

#ifdef WIN32
        std::vector<std::string> strvec;
        boost::algorithm::split(strvec,getenv("USERPROFILE"),boost::is_any_of("\\"));

        for(auto str: strvec){
            Path.push_back(str);
        }
#endif
#ifdef __linux__
        std::vector<std::string> strvec;
        boost::algorithm::split(strvec,getenv("HOME"),boost::is_any_of("/"));

        for(auto str: strvec){
            Path.push_back(str);
        }
#endif
    }

    FileSelectDialog::FileSelectDialog(std::string name,std::vector<std::string> FileType, std::function<void(boost::filesystem::path)> OnOpenFunc, std::function<void(boost::filesystem::path)> OnSaveFunc) : Widget(name, false) {
        Path = std::vector<std::string>();
        OnOpen = std::move(OnOpenFunc);
        OnSave = std::move(OnSaveFunc);
        Extension = FileType;
        SelectedFile = 0;

#ifdef WIN32
        std::vector<std::string> strvec;
        boost::algorithm::split(strvec,getenv("USERPROFILE"),boost::is_any_of("\\"));

        for(auto str: strvec){
            Path.push_back(str);
        }
#endif
#ifdef unix
        std::vector<std::string> strvec;
        boost::algorithm::split(strvec,getenv("HOME"),boost::is_any_of("/"));

        for(auto str: strvec){
            Path.push_back(str);
        }
#endif
    }

    void FileSelectDialog::Draw() {
        const int MaxTextHeight = ImGui::CalcTextSize("abcdefghijklmnopqrstivwxyzABCDEFGHIJKLMNOPQRSTUVW").y;
        boost::filesystem::path CurrentSelectedFile;
        bool CurrentFileIsFolder = false;
        {
            ImGui::BeginGroupPanel("Path", ImVec2(0.9f, 0.0f));
            float yCPos = ImGui::GetCursorPosY();
            float xCPos = ImGui::GetCursorPosX();

            yCPos -= 8;
            ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.0,0.0,0.0,0.0));
            int num = 1;
            for (std::string S: Path) {
                ImGui::SetCursorPos(ImVec2(xCPos, yCPos));
                if(ImGui::Button((S + "/").c_str()))
                    while(num != Path.size())
                        Path.pop_back();
                xCPos += ImGui::CalcTextSize((S + "/").c_str()).x + 8;
                num++;
            }
            ImGui::PopStyleColor();
            ImGui::EndGroupPanel();
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth()-ImGui::CalcTextSize(ICON_FA_MAGNIFYING_GLASS).x - 15);

        if(ImGui::Button(ICON_FA_MAGNIFYING_GLASS))
            ImGui::OpenPopup((std::string("Search##") + GetName().c_str()).c_str());

        if(ImGui::BeginPopup((std::string("Search##") + GetName().c_str()).c_str())){
            ImGui::Text("Search:");
            ImGui::InputText((std::string("##SearchInput") + GetName().c_str()).c_str(),&Filter);
            ImGui::EndPopup();
        }


        int yPrev = ImGui::GetCursorPosY();
#ifdef WIN32
        ImGui::BeginGroupPanel("Drives", ImVec2(0.0f, 0.0f));
        DWORD DriveBitBuf = GetLogicalDrives();
        char DriveLetter = 'A';
        while(DriveBitBuf != 0)
        {
            if(DriveBitBuf & 1){
                if(ImGui::Button(std::string(":/").insert(0,1,DriveLetter).c_str())){
                    Path.clear();
                    Path.push_back(( std::string(1, DriveLetter) + ":/"));
                }
            }

            if(DriveLetter == 'Y')
                break;

            DriveBitBuf = DriveBitBuf >> 1;
            DriveLetter++;
        }
        ImGui::EndGroupPanel();

        ImGui::SetCursorPosY(yPrev);
        ImGui::SetCursorPosX(ImGui::CalcTextSize(" Drives").x+30);
#endif

        ImGui::BeginGroupPanel("Files");
            ImGui::BeginChild("Files",ImVec2(-1.0,ImGui::GetContentRegionMax().y-ImGui::GetCursorPosY()-35));
                boost::filesystem::directory_iterator end_iter;
                int num = 0;
                for (boost::filesystem::directory_iterator itr(GetPath()); itr != end_iter; ++itr) {

                    std::string Text = itr->path().filename().string();
                    boost::algorithm::to_lower(Text);
                    boost::algorithm::to_lower(Filter);
                    if(!boost::algorithm::contains(Text,Filter))
                        continue;

                    bool isfile = true;
                    if (boost::filesystem::is_regular_file(itr->path())) {
                        bool IsValidFiletype = false;
                        for(std::string ext : Extension)
                            if(itr->path().extension() == ext)
                                IsValidFiletype = true;
                        if(!IsValidFiletype)
                            continue;

                        ImGui::Text(ICON_FA_FILE " ");
                        ImGui::SameLine();

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 1.0, 1.0));
                    }
                    else if (boost::filesystem::is_directory(itr->path()))
                    {
                        ImGui::Text(ICON_FA_FOLDER);
                        ImGui::SameLine();

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.5, 0.0, 1.0));
                        if(num == SelectedFile)
                            CurrentFileIsFolder = true;



                        isfile = false;
                    }
                    else
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));

                    if(ImGui::Selectable(itr->path().filename().string().c_str(),num == SelectedFile)) {
                        strcpy(Filename,itr->path().filename().string().c_str());
                        SelectedFile = num;
                    }
                    if(num == SelectedFile)
                        CurrentSelectedFile = itr->path();


                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)){
                        Filter = "";

                        if(!isfile)
                            Path.push_back(itr->path().filename().string());
                        else {
                            if(fileSelectMode == Open)
                                OnOpen(itr->path());
                            else
                                OnSave(itr->path());
                            Active = false;
                        }
                    }

                    ImGui::PopStyleColor();
                    num++;
                }
            ImGui::EndChild();
        ImGui::EndGroupPanel();
        yPrev = ImGui::GetCursorPosY();

        if(fileSelectMode == Open) {
            ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize("Open").x - 10);
            if (ImGui::Button("Open")) {
                if (CurrentFileIsFolder) {
                    Path.push_back(CurrentSelectedFile.filename().string());
                } else {
                    OnOpen(CurrentSelectedFile);
                    Active = false;
                }
            }
        }
        else{
            bool Done = ImGui::InputText("File Name",Filename,256,ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::SetCursorPosY(yPrev);
            ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize("Open").x - 10);

            Done |= ImGui::Button("Save");
            if (Done) {
                OnSave(GetPath() / std::string(Filename));
                Active = false;
            }
        }

        ImGui::SetCursorPosY(yPrev);
        ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - ImGui::CalcTextSize("Open").x - ImGui::CalcTextSize("Close").x - 30);
        if(ImGui::Button("Close")){
            Active = false;
        }
    }

    boost::filesystem::path FileSelectDialog::GetPath() {
        boost::filesystem::path path = boost::filesystem::path();

        for(std::string S: Path){
            path.append(S + "/");
        }
        return path;
    }

    FileSelectDialog::~FileSelectDialog() {

    }


}
