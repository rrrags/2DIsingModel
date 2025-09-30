import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle{
    anchors.fill: parent
    property bool isRunning: false
    Rectangle{
        id: gridRectangle
        anchors.centerIn: parent
        width: 800
        height: 600
        clip: true
        InteractableGrid{
            anchors.fill: parent
            //width: Math.min(gridRectangle.width, 800)
            //height: Math.min(gridRectangle.height, 600)
            //anchors.centerIn: parent
        }
    }
    RowLayout{
        anchors.top: gridRectangle.bottom
        anchors.left: gridRectangle.left
        Button{
            text: isRunning ? "Pause simulation" : "Begin simulation"
            width: 100
            height: 80
            onClicked:{
                isRunning ? gridModel.pauseSimulation() : gridModel.beginSimulation()
                isRunning = !isRunning
            }
        }
        Button{
            text: "newGrid"
            onClicked:{
                showPilot = true
            }
        }
    }
}

