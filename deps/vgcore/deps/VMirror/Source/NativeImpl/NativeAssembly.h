//
// Created by tv on 09.08.23.
//

#ifndef ENGINE_NATIVEASSEMBLY_H
#define ENGINE_NATIVEASSEMBLY_H

#include<string>

#ifdef __unix
#   include"Unix/NativeAssemblyInternals.h"
#elif _WIN64
#   include"Windows/NativeAssemblyInternals.h"
#endif

namespace VMirror{
    class NativeAssembly{
    public:
        NativeAssembly(std::string name);

        void* getExport(const std::string &name);

        std::string m_name;

        virtual ~NativeAssembly();

    private:
        NativeAssemblyInternals m_internals;
    };
}

#endif //ENGINE_NATIVEASSEMBLY_H
