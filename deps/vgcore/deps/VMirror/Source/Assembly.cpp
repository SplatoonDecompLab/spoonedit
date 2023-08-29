#include<Assembly.h>
#include<iostream>
#include<AssemblyRegistry.h>
#include<ReflectableClass.h>

namespace VMirror{



    Assembly::Assembly(std::string name) {
        m_nativeAssembly = std::make_unique<NativeAssembly>(name);

        GetAsmFunc InfoGetter = (GetAsmFunc)m_nativeAssembly->getExport("get"+name+"AssemblyInfo");

        if(InfoGetter == nullptr) {
            std::cout << "Unable to find assembly info!" << std::endl;
            return;
        }
        m_info = InfoGetter();
    }

    std::vector<const ClassInfo *> Assembly::getAllSubclasses(const ClassInfo &info) {
        std::vector<const ClassInfo *> output;

        for(auto& classinfo: m_info->m_classByName){
            if(classinfo.second.isSubclassOf(info))
                output.push_back(&classinfo.second);
        }


        return output;
    }

    FirstAssemblyRegistrationHelper::FirstAssemblyRegistrationHelper(GetAsmFunc getAsmFunc) {
        getAssemblyRegistry()->selfRegister(getAsmFunc());
    }
}