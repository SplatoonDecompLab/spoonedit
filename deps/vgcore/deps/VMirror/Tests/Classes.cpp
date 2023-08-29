#include <gtest/gtest.h>
#include<ReflectableClass.h>

class Base: public VMirror::Reflectable{
public:

    virtual void func() = 0;

    ~Base() override = default;
    VABSTRACTCLASS(Base,ESC({}))
};

REGISTERABSTRACTVCLASS(Base)

class Derived: public Base{
public:
    VCLASS(Derived,ESC({ &Base::classInfo, nullptr }))

    void func() override{

    }

    unsigned volatile int var2 = 2;

    ~Derived() override = default;
};

REGISTERVCLASS(Derived)

class Derived2: public Base{
public:
VCLASS(Derived2,ESC({ &Base::classInfo, nullptr }))

    void func() override{
        std::cout << "pain";
    }

    unsigned volatile int var2 = 2;

    ~Derived2() override = default;
};

REGISTERVCLASS(Derived2)

TEST (Classes, Registration) {
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_classByName.size(),3);
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_classByName.contains("Base"),true);
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_classByName.contains("Derived"),true);
    ASSERT_EQ(VMirror::getCurrentAssembly()->m_info->m_classByName.find("Derived")->second.getImplementations().size(),1);
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_classByName.find("Derived")->second.getImplementations()[0]->m_name, "Base");
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_classByName.find("Base")->second.m_isVirtual, true);
    EXPECT_EQ(VMirror::getCurrentAssembly()->m_info->m_classByName.find("Derived")->second.m_isVirtual, false);
}

TEST (Classes, Construction) {
    VMirror::Reflectable* refl = VMirror::getCurrentAssembly()->m_info->m_classByName.find("Derived")->second.construct();

    ASSERT_NE(refl,nullptr);

    auto* obj = dynamic_cast<Base*>(refl);
    ASSERT_NE(obj,nullptr);

    auto* obj2 = dynamic_cast<Derived*>(obj);
    ASSERT_NE(obj2,nullptr);

    delete refl;
}

TEST (Classes, SubclassChecking) {
    ASSERT_EQ(Derived::classInfo.isSubclassOf(Base::classInfo),true);
    ASSERT_EQ(VMirror::getCurrentAssembly()->getAllSubclasses(Base::classInfo).size(),3);
}
