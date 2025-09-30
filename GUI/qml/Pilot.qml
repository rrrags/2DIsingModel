import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls 2.15


ColumnLayout{
    id: root
    spacing: 50
    TextField {
        id: dimensionTextField
        validator: IntValidator { bottom: 0}
        anchors.centerIn: root
        Layout.preferredHeight: 40
        Layout.preferredWidth: 250
        font.pixelSize: 18
        placeholderText: "Enter the dimension of the grid"
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: "gray"
            radius: 4
        }
    }
    Button{
        Layout.alignment: Qt.AlignCenter
        Layout.preferredHeight: 40
        Layout.preferredWidth: 100
        text: "Begin"
        onClicked:{
            showPilot = false
            gridModel.setGridSize(dimensionTextField.text)
        }
    }

}


