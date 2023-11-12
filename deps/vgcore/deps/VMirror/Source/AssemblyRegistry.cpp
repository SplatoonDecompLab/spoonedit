#include"AssemblyRegistry.h"
#include<algorithm>
#include <iostream>

namespace VMirror{

    std::optional<Assembly*> AssemblyRegistry::loadAssembly(std::string name) {
        auto possibleasmiter = std::find_if(m_assemblies.begin(), m_assemblies.end(),[&](const std::unique_ptr<Assembly> &assembly){
            return name == assembly->m_info->m_name;
        });

        if(possibleasmiter != m_assemblies.end())
            return possibleasmiter->get();

        Assembly* assembly = new Assembly(name);

        if(assembly->m_info->m_automaticRegistry){
            return std::find_if(m_assemblies.begin(), m_assemblies.end(),[&](const std::unique_ptr<Assembly> &assembly){
                return name == assembly->m_info->m_name;
            })->get();
        }

        std::cout << "Registering assembly: " << assembly->m_info->m_name << std::endl;

        m_assemblies.push_back(std::unique_ptr<Assembly>(assembly));
        if(assembly->m_info == nullptr)
            return std::nullopt;

        flushQueue();

        return assembly;
    }

    void AssemblyRegistry::selfRegister(AssemblyInfo *info) {
        std::cout << "Registering assembly: " << info->m_name << std::endl;

        if(std::any_of(m_assemblies.begin(), m_assemblies.end(),[&](const std::unique_ptr<Assembly> &assembly){
            return assembly->m_info->m_name == info->m_name;
        })) {
            std::cout << "Warning: Dual registration of assembly!" << std::endl;
            return;
        }

        m_assemblies.push_back(std::make_unique<Assembly>(info));
    }

    void AssemblyRegistry::flushQueue() {
        std::cout << "Flushing class registration queue" << std::endl;

        while(!m_registrationQueue.empty()){
            m_registrationQueue.front()();
            m_registrationQueue.pop();
        }
    }

    AssemblyRegistry* getAssemblyRegistry(){
        static AssemblyRegistry registry = AssemblyRegistry();

        return &registry;
    }
}