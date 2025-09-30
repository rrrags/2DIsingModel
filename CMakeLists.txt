cmake_minimum_required(VERSION 3.20)

project(2DIsingModel)

enable_testing()

add_library(compile_features INTERFACE)

target_compile_features(compile_features INTERFACE cxx_std_20)

if(MSVC)
    target_compile_options(compile_features INTERFACE /W4)
else()
    target_compile_options(compile_features INTERFACE -Wall -Wextra)
endif()

add_subdirectory(GUI)
add_subdirectory(Lattice)