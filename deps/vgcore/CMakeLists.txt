cmake_minimum_required(VERSION 3.23)

project(VGCore VERSION 0.1.1)

set (CMAKE_CXX_STANDARD 20)

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
if(NOT GCoreDemo)
    add_subdirectory(demo)
endif()

add_library(GCore STATIC "src/Graphics.cpp" "src/Window.cpp" src/MessageBox.cpp src/Mesh.cpp src/Widget.cpp src/Viewport.cpp src/ViewportWidget.cpp src/Shader.cpp src/Texture.cpp src/Sprite.cpp src/FileSelectDialog.cpp src/Outliner.cpp src/EditorPopup.cpp)

target_link_libraries(GCore OpenGL::GL glfw GLEW::GLEW ImGUI glm::glm Boost::filesystem assimp)

target_include_directories(GCore PUBLIC include deps/IconFontCppHeaders)