#include <gtest/gtest.h>
#include"../Source/Assembly.h"

VMIRRORASSEMBLY(test1,VMIRRORVERSION(1,1,1,1),VMirror::AssemblyType::Executable)

TEST (Assembly, AssemblyCreation) {
    EXPECT_EQ (gettest1AssemblyInfo()->m_name, "test1");
    EXPECT_EQ (gettest1AssemblyInfo()->m_type, VMirror::AssemblyType::Executable);
    EXPECT_EQ (gettest1AssemblyInfo()->m_version, (VMIRRORVERSION(1,1,1,1)));
}
