cmake_minimum_required(VERSION 3.23)

project(VGCore VERSION 0.1.1)

set (CMAKE_CXX_STANDARD 20)

ADD_DEFINITIONS(-DASSIMP_BUILD_NO_MDL_IMPORTER)
ADD_DEFINITIONS(-DASSIMP_BUILD_NO_HMP_IMPORTER)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glm)
add_subdirectory(deps/glfw)
add_subdirectory(deps/imgui)
add_subdirectory(deps/assimp)
add_subdirectory(deps/VMirror)

add_subdirectory(deps/vmath)

#if(NOT GCoreDemo)
#    add_subdirectory(demo)
#endif()

add_library(GCore STATIC src/virintox/gcore/Graphics.cpp src/virintox/gcore/gui/Window.cpp src/virintox/gcore/gui/MessageBox.cpp src/virintox/gcore/Mesh.cpp src/virintox/gcore/gui/Widget.cpp src/virintox/gcore/Framebuffer.cpp src/virintox/gcore/gui/ViewportWidget.cpp src/virintox/gcore/Shader.cpp src/virintox/gcore/Texture.cpp src/virintox/gcore/Sprite.cpp src/virintox/gcore/gui/FileSelectDialog.cpp src/virintox/gcore/Material.cpp src/virintox/gcore/Material.h
        src/virintox/gcore/Assembly.cpp
        src/virintox/gcore/gui/MenuItem.h
        src/virintox/gcore/gui/MenuItem.cpp)

target_link_libraries(GCore OpenGL::GL glfw GLEW::GLEW ImGUI glm::glm Boost::filesystem assimp VMath VMirror)


target_compile_definitions(GCore PRIVATE VMIRROR_ASSEMBLYNAME="VGCore")

target_include_directories(GCore PUBLIC src deps/IconFontCppHeaders)