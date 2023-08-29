

#include"Shader.h"
#include<boost/dll.hpp>
#include<boost/algorithm/string.hpp>
#include<iostream>
#include<mutex>
#include<filesystem>

namespace Graphics{

    Shader::Shader(const std::string &name){
        GLuint VShader = glCreateShader(GL_VERTEX_SHADER);

        std::string shaderPath = (std::filesystem::current_path() / "Shaders" / name).string();

        assert(std::filesystem::exists(shaderPath+".vsh") && "Vertex shader for requested shader does not exist!");

        std::ifstream vertexSourceFile(shaderPath + ".vsh");

        std::stringstream vertexSourceStream;
        vertexSourceStream << vertexSourceFile.rdbuf();
        std::string vertexSource = vertexSourceStream.str();

        boost::algorithm::replace_all(vertexSource,"\\n","\n");

        char* vertStr = vertexSource.data();

        glShaderSource(VShader, 1, &vertStr, 0);

        glCompileShader(VShader);

        vertexSourceFile.close();

        GLint isCompiled = 0;
        glGetShaderiv(VShader, GL_COMPILE_STATUS, &isCompiled);

        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(VShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(VShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(VShader);

            for(GLchar c: infoLog)
                std::cout << c;
            std::cout << std::endl;
            throw 1;

            return;
        }

        GLuint FShader = glCreateShader(GL_FRAGMENT_SHADER);

        assert(std::filesystem::exists(shaderPath + ".fsh") && "Vertex shader for requested shader does not exist!");

        std::ifstream fragmentSourceFile(shaderPath + ".fsh");

        std::stringstream fragmentSourceStream;
        fragmentSourceStream << fragmentSourceFile.rdbuf();
        std::string fragmentSource = fragmentSourceStream.str();

        boost::algorithm::replace_all(fragmentSource,"\\n","\n");

        char* fragStr = fragmentSource.data();

        glShaderSource(FShader, 1, &fragStr, 0);

        glCompileShader(FShader);

        glGetShaderiv(FShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(FShader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(FShader, maxLength, &maxLength, &infoLog[0]);

            for(GLchar c: infoLog)
                std::cout << c;
            std::cout << std::endl;
            throw 1;
            glDeleteShader(FShader);
            glDeleteShader(VShader);
            return;
        }

        m_shaderId = glCreateProgram();
        glAttachShader(m_shaderId, VShader);
        glAttachShader(m_shaderId, FShader);
        glLinkProgram(m_shaderId);

        GLint isLinked = 0;
        glGetProgramiv(m_shaderId, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_shaderId, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_shaderId, maxLength, &maxLength, &infoLog[0]);

            for(GLchar c: infoLog)
                std::cout << c;
            std::cout << std::endl;
            throw 1;
            glDeleteProgram(m_shaderId);
            glDeleteShader(VShader);
            glDeleteShader(FShader);
            return;
        }



        glDetachShader(m_shaderId, VShader);
        glDetachShader(m_shaderId, FShader);
        std::cout << "successfully compiled shader\n";

        glUseProgram(Shader::m_shaderId);

        GLint texture = glGetUniformLocation(Shader::m_shaderId, "TextureSampler");

        glUniform1i(texture,0);

        GLint normalMap = glGetUniformLocation(Shader::m_shaderId, "NormalMapSampler");

        glUniform1i(normalMap,1);

        GLint specMap = glGetUniformLocation(Shader::m_shaderId, "SpecularMapSampler");

        glUniform1i(specMap,2);

        GLint emmissionMap = glGetUniformLocation(Shader::m_shaderId, "EmmissionMapSampler");

        glUniform1i(emmissionMap,3);

        GLint teamcolorMap = glGetUniformLocation(Shader::m_shaderId, "TeamcolorMap");

        glUniform1i(teamcolorMap,4);

        GLint shadowMap = glGetUniformLocation(Shader::m_shaderId, "ShadowMapSampler");

        glUniform1i(shadowMap,5);


    }

    void Shader::use() {
        glUseProgram(m_shaderId);
    }

    Shader::~Shader() {
        //glDeleteProgram(m_shaderId);
    }

    GLint Shader::getUniformLocation(const char* name) {
        GLint location = glGetUniformLocation(m_shaderId,name);

        return location;
    }
}