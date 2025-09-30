import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 1000
    height: 800
    visible: true
    title: "Game of Life"
    property bool showPilot: true

    Loader{
        id: viewLoader
        anchors.centerIn: parent
        source: showPilot ? "Pilot.qml" : "ControlBoard.qml"
    }

}
