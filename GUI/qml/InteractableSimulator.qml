import QtQuick 2.15
import QtQuick.Controls 2.15
import simulator 1.0

Item{
    property int cellSize: 10
    property int latticeSize: latticeModel.lattice.latticeSize
    property real scaleFactor: 1.0

    Item {
        id: zoomWrapper
        anchors.fill: parent
        transform: Scale {
            id: zoomScale
            origin.x: zoomWrapper.width / 2
            origin.y: zoomWrapper.height / 2
            xScale: scaleFactor
            yScale: scaleFactor
        }


        WheelHandler {
                id: wheelHandler
                target: zoomWrapper
                onWheel: (wheel) => {
                    const zoomStep = 0.5;
                    const delta = wheel.angleDelta.y > 0 ? zoomStep : -zoomStep;
                    scaleFactor = Math.max(0.2, Math.min(5.0, scaleFactor + delta));
                }

        }

        // use TableView instead?
        LatticeView {
            id: lattice
            anchors.centerIn: parent
            model: latticeModel
            width: cellSize * latticeSize
            height: cellSize * latticeSize
            clip: true
            cellWidth: cellSize
            cellHeight: cellSize
            delegate: Rectangle {
                width: cellSize
                height: cellSize
                color: model.cell ===  GOLLattice.DEAD ? "red" : "green"
                border.color: "#444"
                border.width: 0.5

                MouseArea {
                       anchors.fill: parent
                       acceptedButtons: Qt.LeftButton | Qt.RightButton
                       onClicked: function(mouse){
                           if (mouse.button === Qt.LeftButton) {
                               //console.log("Clicked cell at index:", index)
                               latticeModel.makeCellAlive(index)
                           } else if (mouse.button === Qt.RightButton) {
                               latticeModel.makeCellDead(index)
                           }
                       }
                   }
            }
        }

    }
}
