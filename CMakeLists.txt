cmake_minimum_required(VERSION 3.25)
project(SpoonTool)

set (CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS thread chrono)
find_package(Lua REQUIRED)

add_subdirectory(deps/vgcore)

add_subdirectory(Tools/MapTools)

add_executable(SpoonEdit src/main.cpp src/MainWindow.cpp src/MainWindow.h src/PropertiesWidget.cpp src/PropertiesWidget.h src/ObjectSelectWidget.cpp src/ObjectSelectWidget.h src/MainViewport.cpp src/MainViewport.h src/Model.cpp src/Model.h src/RailSelectWidget.cpp src/RailSelectWidget.h src/Config/Configs.h src/Config/Configs.cpp)

target_link_libraries(SpoonEdit PUBLIC GCore SpoonMapTools Boost::thread Boost::chrono)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

install(TARGETS SpoonEdit
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug
        LIBRARY DESTINATION Debug)

install(TARGETS SpoonEdit
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release
        LIBRARY DESTINATION Release)