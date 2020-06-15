import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id: edge

    property var edgesID
    property bool textVisible: true
    property real fromX: 0
    property real toX: 10
    property real fromY: 0
    property real toY: 10

    x: fromX
    y: fromY
    z: 0
    height: diffY
    width: diffX
    //opacity: 0
    rotation: 0

    property real diffX: toX-fromX
    property real diffY: toY-fromY
    property real d: Math.sqrt(Math.pow(diffX, 2) + Math.pow(diffY, 2))
    property real rad: Math.atan2(diffY, diffX)

    Rectangle {
        id: line
        height: 5
        width: 0.9
        antialiasing: true
        transform: Matrix4x4 {
            matrix: Qt.matrix4x4(d*Math.cos(rad), -Math.sin(rad), 0, 0,
                             d*Math.sin(rad), Math.cos(rad), 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1)
        }
    }

    Rectangle {
        visible: textVisible
        width: 40
        height: 40
        x: diffX/2
        y: diffY/2

        TextField {
            id: text_field
            background: Rectangle {}
            anchors.centerIn: parent
            activeFocusOnPress: false
            selectByMouse: true
            selectedTextColor: "#ee1c1c"
            text: weight != undefined ? weight : ""
            onEditingFinished: weight = text
            onFocusChanged: mouse_area.enabled = !text_field.focus
        }

        MouseArea {
            id: mouse_area
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                    if (pressedButtons & Qt.LeftButton) {
                        if(parent.activeFocus == true) {
                            text_field.focus = true
                            text_field.cursorPosition = text_field.positionAt(mouseX)
                        }
                        else if (text_field.activeFocus == false) {
                            parent.focus = true
                        }
                    } else if (pressedButtons & Qt.RightButton) {
                        edgesID.remove(label)
                    }
            }
        }
    }
}
