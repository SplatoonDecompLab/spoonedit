//
// Created by tv on 09.08.23.
//

#ifndef ENGINE_NATIVEASSEMBLYIMPL_H
#define ENGINE_NATIVEASSEMBLYIMPL_H


#include "NativeImpl/NativeAssembly.h"
#include<filesystem>
#include<iostream>
#include<dlfcn.h>

namespace VMirror{
    NativeAssembly::NativeAssembly(std::string name) {
        std::filesystem::path selfpath =  std::filesystem::canonical("/proc/self/exe").parent_path();


#ifdef __unix
        std::string asmpath = selfpath / ("lib" + name + ".so");
#endif

        m_internals.m_assembly = dlopen(asmpath.c_str(), RTLD_NOW | RTLD_GLOBAL);

        if(m_internals.m_assembly == nullptr)
            std::cout << "Unable to load shared object, continuing anyways!" << std::endl;

        m_name = name;
    }

    void *NativeAssembly::getExport(const std::string &name) {
        return dlsym(m_internals.m_assembly,name.c_str());
    }

    NativeAssembly::~NativeAssembly() {
        dlclose(m_internals.m_assembly);
    }
}

#endif //ENGINE_NATIVEASSEMBLYIMPL_H
