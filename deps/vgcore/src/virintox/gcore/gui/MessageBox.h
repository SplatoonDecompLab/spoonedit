#pragma once
#include "Widget.h"
#include <exception>
namespace Graphics{

    enum ErrorSeverity{
        Fault,
        Error,
        Warning,
        Info
    };

    class MSGBoxException : public std::exception{
    public:
        inline MSGBoxException(std::string What,ErrorSeverity severity): whattext(What),Severity(severity){};

        const char * what() const noexcept override;

        std::string whattext;
        ErrorSeverity Severity; 
    };

    class MessageBox : public Widget{
    public:
        VABSTRACTCLASS(MessageBox,ESC({&Widget::classInfo,nullptr}))

        MessageBox(std::string Text, ErrorSeverity Severity = Info);
        void Draw() override;
        void PostDraw() override;
        bool Deactivate = false;
    private:
        std::string text;
        ErrorSeverity severity;
        
    public:
        ~MessageBox() override;
    };
}