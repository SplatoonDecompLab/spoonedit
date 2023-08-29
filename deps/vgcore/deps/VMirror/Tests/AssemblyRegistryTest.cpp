#undef VMIRROR_SELF_OVERRIDE
#include <gtest/gtest.h>
#include"AssemblyRegistry.h"

TEST (Registry, AutoRegistration) {
    EXPECT_EQ(VMirror::getAssemblyRegister()->m_assemblies.size(), 1);
    EXPECT_EQ(VMirror::getAssemblyRegister()->m_assemblies[0]->m_info->m_name, "VMirrorTest");
    EXPECT_EQ(VMirror::getAssemblyRegister()->m_assemblies[0]->m_info->m_version, (VMIRRORVERSION(1,0,0,0)));
    EXPECT_EQ(VMirror::getAssemblyRegister()->m_assemblies[0]->m_info->m_type, VMirror::AssemblyType::Executable);
}

TEST (Registry, SelfAssemblyReference) {
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_name, "VMirrorTest");
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_version, (VMIRRORVERSION(1,0,0,0)));
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_type, VMirror::AssemblyType::Executable);
}


