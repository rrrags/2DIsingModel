import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Rectangle{
    anchors.fill: parent
    property bool isRunning: false
    Rectangle{
        id: latticeRectangle
        anchors.centerIn: parent
        width: 800
        height: 600
        clip: true
        InteractableSimulator{
            anchors.fill: parent
            //width: Math.min(latticeRectangle.width, 800)
            //height: Math.min(latticeRectangle.height, 600)
            //anchors.centerIn: parent
        }
    }
    RowLayout{
        anchors.top: latticeRectangle.bottom
        anchors.left: latticeRectangle.left
        Button{
            text: isRunning ? "Pause simulation" : "Begin simulation"
            width: 100
            height: 80
            onClicked:{
                isRunning ? latticeModel.pauseSimulation() : latticeModel.beginSimulation()
                isRunning = !isRunning
            }
        }
        Button{
            text: "newSimulator"
            onClicked:{
                showPilot = true
            }
        }
    }
}

