import QtQuick 2.12
//import "factory.js" as Factory

Rectangle {
    //property var modelID
    //property var dummyID

    width: 40
    height: 40
    anchors.verticalCenter: parent.verticalCenter

    border {
        color: "black"
        width: 1
    }

    color: "white"

    Text {
        anchors.centerIn: parent
        text: labelAlph + " | " + weight
    }

    /*MouseArea {
        id: other
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (other.pressedButtons & Qt.LeftButton)
                modelID.append(dummyID.createObject(null, {}), index, 2)
            else if (other.pressedButtons & Qt.RightButton)
                Factory.removeVertex(modelID, index)
        }
    }*/
}
