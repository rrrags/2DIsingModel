add_library(lattice STATIC
    lattice.hpp
    lattice.cpp
)

target_include_directories(lattice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lattice PRIVATE compile_features)