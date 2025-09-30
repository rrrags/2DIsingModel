
find_package(Qt6  CONFIG REQUIRED COMPONENTS Core Gui Qml Quick Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(GOL_SOURCES
  latticeModel.hpp
  latticeModel.cpp
  main.cpp
)

set(GOL_QRCS
  qml/resource.qrc
)
qt_add_resources(GOL_SOURCES ${GOL_QRCS})

add_executable(GUI ${GOL_SOURCES}
)
target_link_libraries(GUI
  PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick

    lattice
    compile_features
)
