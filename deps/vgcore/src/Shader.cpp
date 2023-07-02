

#include<virintox/gcore/Shader.h>
#include<boost/dll.hpp>
#include<iostream>
#include<mutex>

namespace Graphics{


    bool initd = false;

    const GLchar *VertShaderSrc2D = "#version 460\n"
                                  "layout (location = 0) in vec2 aPos;"
                                  "layout (location = 1) in vec2 aTexCoord;\n"
                                  "uniform mat4 ProjectionMatrix;\n"
                                  "uniform mat4 TransformationMatrix;\n"
                                  "out vec2 TexCoord;\n"
                                  "void main(){\n"
                                  "gl_Position = ProjectionMatrix * (vec4(aPos.x,aPos.y,1.0,1.0) * TransformationMatrix);"
                                  "TexCoord = aTexCoord;\n"
                                  "}\n";

    const GLchar *FragShaderSrc2D = "#version 460\n"
                                  "in vec2 TexCoord;\n"
                                  "out vec4 FragColor;\n"
                                  "uniform sampler2D tex;\n"
                                  "void main(){\n"
                                  "FragColor =  texture(tex,TexCoord);//vec4(1.0f,1.0f,1.0f,1.0f);\n"
                                  "}\n";

    const GLchar *VertShaderSrc3D = "#version 460\n"
                                  "layout (location = 0) in vec2 aPos;"
                                  "layout (location = 1) in vec2 aTexCoord;\n"
                                  "uniform mat4 ProjectionMatrix;\n"
                                  "out vec2 TexCoord;\n"
                                  "void main(){\n"
                                  "gl_Position = ProjectionMatrix * vec4(aPos.x,-aPos.y,1.0,1.0);"
                                  "TexCoord = aTexCoord;\n"
                                  "}\n";

    const GLchar *FragShaderSrc3D = "#version 460\n"
                                  "in vec2 TexCoord;\n"
                                  "out vec4 FragColor;\n"
                                  "uniform sampler2D tex;\n"
                                  "void main(){\n"
                                  "FragColor =  texture(tex,TexCoord);//vec4(1.0f,1.0f,1.0f,1.0f);\n"
                                  "}\n";

    Shader::Shader(const GLchar* VertexSrc, const GLchar* FragmentSrc){
        GLuint VShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(VShader, 1, &VertexSrc, 0);

        glCompileShader(VShader);

        GLint isCompiled = 0;
        glGetShaderiv(VShader, GL_COMPILE_STATUS, &isCompiled);

        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(VShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(VShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(VShader);

            // Use the infoLog as you see fit.

            for(GLchar c: infoLog)
                std::cout << c;
            std::cout << std::endl;
            throw 1;

            // In this simple program, we'll just leave
            return;
        }

        // Create an empty fragment shader handle
        GLuint FShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        glShaderSource(FShader, 1, &FragmentSrc, 0);

    // Compile the fragment shader
        glCompileShader(FShader);

        glGetShaderiv(FShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(FShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(FShader, maxLength, &maxLength, &infoLog[0]);

            for(GLchar c: infoLog)
                std::cout << c;
            std::cout << std::endl;
            throw 1;
            // We don't need the shader anymore.
            glDeleteShader(FShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(VShader);

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            return;
        }

        ShaderId = glCreateProgram();
        glAttachShader(ShaderId,VShader);
        glAttachShader(ShaderId,FShader);
        glLinkProgram(ShaderId);

        GLint isLinked = 0;
        glGetProgramiv(ShaderId, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(ShaderId, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(ShaderId, maxLength, &maxLength, &infoLog[0]);

            for(GLchar c: infoLog)
                std::cout << c;
            std::cout << std::endl;
            throw 1;
            // We don't need the program anymore.
            glDeleteProgram(ShaderId);
            // Don't leak shaders either.
            glDeleteShader(VShader);
            glDeleteShader(FShader);

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            return;
        }

        GLuint texture = glGetUniformLocation(Shader::ShaderId, "tex");

        glUniform1i(texture,0);

        glDetachShader(ShaderId, VShader);
        glDetachShader(ShaderId, FShader);
        std::cout << "successfully compiled shader\n";


    }

    void Shader::Use() {
        glUseProgram(ShaderId);
    }

    Shader &GetDefaultShader2D(){
        static Shader DefaultShader2D = Shader(VertShaderSrc2D, FragShaderSrc2D);
        return DefaultShader2D;
    }
    
    Shader &GetDefaultShader3D(){
        static Shader DefaultShader3D = Shader(VertShaderSrc3D, FragShaderSrc3D);
        return DefaultShader3D;
    }
}