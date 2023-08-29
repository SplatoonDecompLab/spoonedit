cmake_minimum_required(VERSION 3.25)
project(VMirror)

set(CMAKE_CXX_STANDARD 20)


add_library(VMirror SHARED
        Source/AssemblyRegistry.cpp
        Source/AssemblyRegistry.h
        Source/NativeImpl/NativeAssembly.h
        Source/NativeImpl/NativeAssembly.cpp
        Source/Assembly.cpp
)

target_include_directories(VMirror PUBLIC Source)

target_compile_definitions(VMirror PRIVATE VMIRROR_SELF_OVERRIDE)

#enable_testing()
#find_package(GTest REQUIRED)

#include(GoogleTest)

#add_executable(VMirrorTests Tests/AssemblyTests.cpp
#        Tests/AssemblyRegistryTest.cpp
#        Tests/AppAssembly.cpp
#        Tests/Classes.cpp)

#target_link_libraries(VMirrorTests VMirror GTest::gtest_main)

#target_compile_definitions(VMirrorTests PRIVATE VMIRROR_ASSEMBLYNAME="VMirrorTest")

#gtest_discover_tests(VMirrorTests)