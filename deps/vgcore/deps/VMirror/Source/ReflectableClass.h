//
// Created by tv on 09.08.23.
//

#ifndef ENGINE_REFLECTABLECLASS_H
#define ENGINE_REFLECTABLECLASS_H

#include<vector>
#include<AssemblyRegistry.h>
#include<functional>
#include<type_traits>
#include<iostream>

namespace VMirror{

    class Reflectable;
    struct ClassInfo;

    typedef Reflectable*(*constructionFunc)();

    struct ClassInfo{
        const std::string m_name;
        const constructionFunc construct;
        const ClassInfo *const *const m_implements;
        const bool m_isVirtual;

        inline bool operator==(const ClassInfo &info) const{
            return m_name == info.m_name;
        }

        inline std::vector<const ClassInfo*> getImplementations() const{
            std::vector<const ClassInfo*> output = std::vector<const ClassInfo*>();

            const ClassInfo *const * ptr = m_implements;
            while (*ptr != nullptr){
                output.push_back(*ptr);
                ptr++;
            }

            return output;
        }

        [[nodiscard]] inline bool isSubclassOf(const ClassInfo &info) const{
            if(*this == info)
                return true;

            if(getImplementations().empty())
                return false;
            return std::ranges::any_of(getImplementations(),[&](const ClassInfo* subclass){
                return subclass->isSubclassOf(info);
            });
        }
    };
    template<typename t>
    class ClassRegistrationHelper{
    public:
        inline ClassRegistrationHelper(){
            std::cout << "Adding class \"" << t::classInfo.m_name << "\" " << "of assembly \"" << VMIRROR_ASSEMBLYNAME << "\" to registration queue." << std::endl;

            getAssemblyRegistry()->m_registrationQueue.push([]() {
                CURRENTASSEMBLY()->m_info->m_classByName.insert({t::classInfo.m_name, t::classInfo});
            });
        }
    };

    class Reflectable{
    public:
        virtual const ClassInfo& getClassInfo() const = 0;
        virtual ~Reflectable() = default;
    };
}

#define ESC(...) __VA_ARGS__

#define VCLASS(name,baseclasses) \
    public:                          \
    static constexpr const char* VMirrorName = #name; \
    static constexpr const VMirror::ClassInfo* VMirrorBaseclasses[] = baseclasses; \
    static constexpr bool VMirrorIsVirtual = false;                             \
    const VMirror::ClassInfo& getClassInfo() const override; \
    static VMirror::Reflectable* construct();\
    static inline VMirror::ClassInfo classInfo = VMirror::ClassInfo{VMirrorName, construct, VMirrorBaseclasses, VMirrorIsVirtual};

#define VABSTRACTCLASS(name,baseclasses) \
    public:                          \
    static constexpr const char* VMirrorName = #name; \
    static constexpr const VMirror::ClassInfo* VMirrorBaseclasses[] = baseclasses; \
    static constexpr bool VMirrorIsVirtual = true;\
    const VMirror::ClassInfo& getClassInfo() const override; \
    static VMirror::Reflectable* construct();\
    static inline VMirror::ClassInfo classInfo = VMirror::ClassInfo{VMirrorName, construct, VMirrorBaseclasses, VMirrorIsVirtual};


#define REGISTERVCLASS(name) \
    static_assert(!std::is_abstract_v<name>); \
    static_assert(!name::VMirrorIsVirtual);\
    const VMirror::ClassInfo& name::getClassInfo() const{ \
        return classInfo;\
    }                       \
    VMirror::Reflectable* name::construct(){                 \
        return new name();                        \
    }                       \
    volatile static VMirror::ClassRegistrationHelper<name> name##RegistrationHelper = VMirror::ClassRegistrationHelper<name>();

#define REGISTERABSTRACTVCLASS(name) \
    static_assert(name::VMirrorIsVirtual);\
    const VMirror::ClassInfo& name::getClassInfo() const{ \
        return classInfo;\
    }                       \
    VMirror::Reflectable* name::construct(){              \
        return nullptr;                        \
    }                       \
    static VMirror::ClassRegistrationHelper<name> name##RegistrationHelper = VMirror::ClassRegistrationHelper<name>();


#endif //ENGINE_REFLECTABLECLASS_H
