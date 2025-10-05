
find_package(Qt6  CONFIG REQUIRED COMPONENTS Core Gui Qml Quick Widgets Concurrent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(2D_ising_model_sources
  simulator.hpp
  simulator.cpp
  latticeModel.hpp
  latticeModel.cpp
  main.cpp
)

set(2d_ising_model_QRCS
  qml/resource.qrc
)
qt_add_resources(2D_ising_model_sources ${2d_ising_model_QRCS})

add_executable(2D_ising_model ${2D_ising_model_sources}
)
target_link_libraries(2D_ising_model
  PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::Concurrent

    compile_features
)
