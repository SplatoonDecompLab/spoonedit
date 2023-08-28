cmake_minimum_required(VERSION 3.25)
project(SpoonTool)

set (CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS thread chrono)

add_subdirectory(deps/vgcore)

add_subdirectory(Tools/MapTools)

add_executable(SpoonEdit src/main.cpp src/MainWindow.cpp src/MainWindow.h src/PropertiesWidget.cpp src/PropertiesWidget.h src/ObjectSelectWidget.cpp src/ObjectSelectWidget.h src/MainViewport.cpp src/MainViewport.h src/Model.cpp src/Model.h src/RailSelectWidget.cpp src/RailSelectWidget.h src/Config/Configs.h src/Config/Configs.cpp)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

target_link_libraries(SpoonEdit PUBLIC GCore SpoonMapTools Boost::thread Boost::chrono)

install(TARGETS SpoonEdit
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug
        LIBRARY DESTINATION Debug)

install(TARGETS SpoonEdit
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release
        LIBRARY DESTINATION Release)