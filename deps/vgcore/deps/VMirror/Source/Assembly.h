//
// Created by tv on 09.08.23.
//

#ifndef VMIRROR_ASSEMBLY_H
#define VMIRROR_ASSEMBLY_H

#include<memory>
#include<NativeImpl/NativeAssembly.h>
#include<unordered_map>
#include<vector>

#if !defined(VMIRROR_ASSEMBLYNAME) && !defined(VMIRROR_SELF_OVERRIDE)
#   error No assembly name specified via compiler
#endif

#ifdef VMIRROR_SELF_OVERRIDE
#   define VMIRROR_OVERRIDE true
#   define VMIRROR_ASSEMBLYNAME "NONE"
#   pragma message ( "VMirror self override is enabled THIS IS ONLY FOR USE BY VMIRROR ITSELF" )
#endif

namespace VMirror{

    struct ClassInfo;


    struct AssemblyVersion{
        const unsigned short m_major;
        const unsigned short m_minor;
        const unsigned short m_revision;
        const unsigned short m_patch;

        inline bool operator ==(const AssemblyVersion &other) const{
            return (other.m_major == m_major) &&
                    (other.m_minor == m_minor) &&
                    (other.m_revision == m_revision) &&
                    (other.m_patch == m_patch);
        }
    };

    enum class AssemblyType{
        Executable = 0b10,
        DynamicLibrary = 0b01,
        StaticLibrary = 0b11
    };

    struct AssemblyInfo{
        const std::string m_name;
        const AssemblyVersion m_version;
        const AssemblyType m_type;
        const bool m_automaticRegistry;
        //Class register
        std::unordered_map<std::string,const ClassInfo&> m_classByName;
    };

    struct Assembly{
        Assembly(std::string name);

        inline Assembly(AssemblyInfo* info){
            m_info = info;
        }

        AssemblyInfo* m_info;
        std::unique_ptr<NativeAssembly> m_nativeAssembly;

        std::vector<const ClassInfo*> getAllSubclasses(const ClassInfo& info);
    };

    typedef AssemblyInfo*(*GetAsmFunc)();

    struct FirstAssemblyRegistrationHelper{
        FirstAssemblyRegistrationHelper(GetAsmFunc getAsmFunc);
    };

}



#define VMIRRORASSEMBLY(name,version,type) \
extern "C"{                                                                                             \
    VMirror::AssemblyInfo* get##name##AssemblyInfo(){                                                   \
        static VMirror::AssemblyInfo name##AssemblyInfo = VMirror::AssemblyInfo{ #name,version,type,false };  \
        return & name##AssemblyInfo;                                                                    \
    }                                                                                                   \
}

#define VMIRRORASSEMBLYSELFREGISTER(name,version,type) \
extern "C"{                                                                                             \
    VMirror::AssemblyInfo* get##name##AssemblyInfo(){                                                   \
        static VMirror::AssemblyInfo name##AssemblyInfo = VMirror::AssemblyInfo{ #name,version,type,false };  \
        return & name##AssemblyInfo;                                                                    \
    }                                                                                                   \
}                                                      \
volatile static VMirror::FirstAssemblyRegistrationHelper name##RegistrationHelper = VMirror::FirstAssemblyRegistrationHelper(get##name##AssemblyInfo);

#define VMIRRORVERSION(major,minor,revision,patch) VMirror::AssemblyVersion{major,minor,revision,patch}

#endif //VMIRROR_ASSEMBLY_H
