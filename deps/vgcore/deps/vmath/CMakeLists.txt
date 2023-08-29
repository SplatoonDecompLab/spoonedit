cmake_minimum_required(VERSION 3.23)

project(VMath VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 20)

add_library(VMath INTERFACE src/virintox/math/Vector2.h src/virintox/math/Vector3.h src/virintox/math/Vector4.h)

target_include_directories(VMath INTERFACE src)