project(GCoreDemo)

set (CMAKE_CXX_STANDARD 20)

add_executable(GCoreDemo "src/main.cpp" "src/Demo.cpp" "src/DemoViewport.cpp" "src/fps.cpp")

target_include_directories(GCoreDemo PUBLIC include)

target_link_libraries(GCoreDemo GCore)