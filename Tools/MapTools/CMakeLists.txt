cmake_minimum_required(VERSION 3.25)
project(SpoonMapTools)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS filesystem iostreams)

find_package(GLEW REQUIRED)

find_package(yaml-cpp REQUIRED)

add_library(SpoonMapTools STATIC src/Map.cpp include/Map.h include/LevelObject.h include/Transform.h "include/Vector3.h" src/Vectior3.cpp src/Transform.cpp src/LevelObject.cpp include/Link.h src/Link.cpp include/Rail.h src/Rail.cpp include/RailPoint.h src/RailPoint.cpp include/SpecialObjectParams.h src/SpecialObjectParams.cpp include/SpecialObjectParamVersions/All.h include/SpecialObjectParamVersions/Default.h  include/Element.h src/Element.cpp)

target_include_directories(SpoonMapTools PUBLIC include)

target_link_libraries(SpoonMapTools PUBLIC Boost::filesystem Boost::iostreams GLEW::GLEW yaml-cpp)

