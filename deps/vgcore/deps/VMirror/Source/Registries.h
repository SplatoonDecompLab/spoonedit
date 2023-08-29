//
// Created by tv on 11.08.23.
//

#ifndef ENGINE_REGISTRIES_H
#define ENGINE_REGISTRIES_H
#include<vector>
#include"AssemblyRegistry.h"
#include"ReflectableClass.h"
#include<type_traits>

namespace VMirror{
    class RegistryBase;

    inline std::vector<RegistryBase*> g_registries;

    class RegistryBase{
    public:
        virtual void collectObjects() = 0;

        virtual unsigned long size() = 0;
        virtual const VMirror::ClassInfo* getTargetClass() = 0;
    };

    template<typename regclass>
    class Registry: public RegistryBase{
    public:
        static_assert(std::is_base_of_v<VMirror::Reflectable,regclass>);

        std::vector<const VMirror::ClassInfo*> m_matchedClasses;
        Registry(){
            g_registries.push_back(this);
        }

        unsigned long size() override {
            return m_matchedClasses.size();
        }

        const VMirror::ClassInfo *getTargetClass() override {
            return &regclass::classInfo;
        }

        void collectObjects() override {
            m_matchedClasses.clear();

            for(const std::unique_ptr<VMirror::Assembly> &assembly: VMirror::getAssemblyRegistry()->m_assemblies){
                std::vector<const VMirror::ClassInfo*> classes = assembly->getAllSubclasses(regclass::classInfo);
                for(const VMirror::ClassInfo* klass: classes){
                    if(klass->m_isVirtual || *klass == regclass::classInfo)
                        continue;
                    m_matchedClasses.push_back(klass);
                }
            }

            std::cout << "Collected " << m_matchedClasses.size() << " classes of type " << regclass::VMirrorName << std::endl;
        }
    };

    inline void g_updateAllRegistries(){
        for(RegistryBase* registry: g_registries){
            registry->collectObjects();
        }
    }
}

#define VREGISTRY(typename) inline VMirror::Registry<typename> g_##typename##Registry = VMirror::Registry<typename>();

#endif //ENGINE_REGISTRIES_H
