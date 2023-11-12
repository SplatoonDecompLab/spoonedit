//
// Created by tv on 04.09.23.
//

#ifndef SPOONTEXTEDIT_NATIVEASSEMBLY_H
#define SPOONTEXTEDIT_NATIVEASSEMBLY_H

#include "NativeImpl/NativeAssembly.h"
#include<windows.h>
#include<filesystem>
#include<iostream>

//Stubbed out

namespace VMirror{
    NativeAssembly::NativeAssembly(std::string name) {

    }

    void *NativeAssembly::getExport(const std::string &name) {
        return nullptr;
    }

    NativeAssembly::~NativeAssembly() {
    }
}


#endif //SPOONTEXTEDIT_NATIVEASSEMBLY_H
