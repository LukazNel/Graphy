import QtQuick 2.15
//import "factory.js" as Factory

Rectangle {
    //property var modelID
    //property var dummyID

    width: 40
    height: 40

    border {
        color: "blue"
        width: 1
    }

    color: "orange"

    Text {
        anchors.centerIn: parent
        text: labelweight
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
