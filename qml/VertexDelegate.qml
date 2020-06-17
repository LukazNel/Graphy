import QtQuick 2.12
import QtQuick.Controls.Material 2.12
//import custom 1.0
//import "factory.js" as Factory


Row {
    //id: root
    //property var modelID
    //property var dummyID

    //width:
    height: 50

    Rectangle {
        id: labelBlock
        color: "white"
        height: parent.height
        width: parent.height
        border { color: "black"; width: 1 }

        Text {
            anchors.centerIn: parent
            text: labelAlph
        }

        /*MouseArea {
            id: other
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                if (other.pressedButtons & Qt.LeftButton)
                    edges.append(dummyID.createObject(null, {}), index, 2)
                else if (other.pressedButtons & Qt.RightButton)
                    modelID.remove(0)
            }
        }*/
    }

    Rectangle {
        width: parent.width - labelBlock.width
        height: parent.height
        color: "white"
        ListView {
            anchors.fill: parent
            orientation: ListView.Horizontal
            model: edges
            spacing: 2
            delegate: EdgeDelegate {/*Component.onCompleted: {modelID = edges}*/}
            //focus: true
        }
    }
}
