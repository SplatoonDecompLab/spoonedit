//VMIRROR_ASSEMBLYNAME
// Created by tv on 09.08.23.
//

#ifndef ENGINE_ASSEMBLYREGISTRY_H
#define ENGINE_ASSEMBLYREGISTRY_H

#include"Assembly.h"
#include<vector>
#include<queue>
#include<optional>
#include<algorithm>
#include<functional>

namespace VMirror{
    class AssemblyRegistry{
    public:

        void selfRegister(AssemblyInfo* info);

        void flushQueue();

        std::optional<Assembly*> loadAssembly(std::string name);

        std::vector<std::unique_ptr<Assembly>> m_assemblies;

        std::queue<std::function<void()>> m_registrationQueue;
    };

    AssemblyRegistry* getAssemblyRegistry();

#define CURRENTASSEMBLY() std::find_if(VMirror::getAssemblyRegistry()->m_assemblies.begin(),\
            VMirror::getAssemblyRegistry()->m_assemblies.end(),[&](const std::unique_ptr<VMirror::Assembly> &assembly1){\
                return assembly1->m_info->m_name == VMIRROR_ASSEMBLYNAME;\
            })->get()
}

#endif //ENGINE_ASSEMBLYREGISTRY_H
