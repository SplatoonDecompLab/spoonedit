#ifndef PNS_SHADER_H
#define PNS_SHADER_H
#include <GL/glew.h>
namespace Graphics{
    class Shader{
    public:
        static void SetupDefaults();

        inline Shader(){ init = false; };
        Shader(const GLchar* Vertex, const GLchar* Fragment);
        void Use();
        GLuint ShaderId;

        bool init;
    };
    Shader &GetDefaultShader2D();
    Shader &GetDefaultShader3D();
}

#endif //PNS_SHADER_H
