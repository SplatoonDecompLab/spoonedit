project(ImGUI)

find_package(GLEW REQUIRED)

add_library(ImGUI STATIC imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp imgui/misc/cpp/imgui_stdlib.cpp)

target_link_libraries(ImGUI glfw GLEW::GLEW)
target_include_directories(ImGUI PUBLIC imgui)