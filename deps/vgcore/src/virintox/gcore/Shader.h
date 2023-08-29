#ifndef PNS_SHADER_H
#define PNS_SHADER_H
#include <GL/glew.h>
#include<string>
#include<map>

namespace Graphics{
    class Shader{
    public:
    public:
        Shader(const std::string &name);
        void use();

        GLint getUniformLocation(const char* name);

        ~Shader();

    private:

        GLint m_shaderId;
    };
}

#endif //PNS_SHADER_H
