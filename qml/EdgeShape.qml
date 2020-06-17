import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

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
    rotation: 0

    property real diffX: toX-fromX
    property real diffY: toY-fromY
    property real d: Math.sqrt(Math.pow(diffX, 2) + Math.pow(diffY, 2))
    property real rad: Math.atan2(diffY, diffX)

    Matrix4x4 {
        id: rotator
        matrix: Qt.matrix4x4(
                    Math.cos(rad), -Math.sin(rad), 0, 0,
                    Math.sin(rad), Math.cos(rad), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1)
    }
    Matrix4x4 {
        id: transformer
        matrix: Qt.matrix4x4(
                    d*Math.cos(rad), -Math.sin(rad), 0, 0,
                    d*Math.sin(rad), Math.cos(rad), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1)
    }
    Matrix4x4 {
        id: shifter
        matrix: Qt.matrix4x4(
                    1, 0, 0, d/4 + 25,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1)
    }

    Rectangle {
        id: line
        height: 5
        width: 0.9
        antialiasing: true
        color: "black"
        transform: transformer
    }

    Rectangle {
        id: labelBlock
        visible: textVisible
        width: 30
        height: 30
        color: "white"
        x: 0; y: 0
        radius: width/2
        transform: Matrix4x4 {
            matrix: rotator.matrix.times(shifter.matrix.times(rotator.matrix.inverted()))
        }

        Label {
            id: text_field
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            background: Rectangle {}
            font.pixelSize: 15
            text: weight != undefined ? weight : ""
        }

        MouseArea {
            id: mouse_area
            property int acceptedKeys: Qt.Key_0

            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                    if (pressedButtons & Qt.LeftButton) {
                        focus = true
                    } else if (pressedButtons & Qt.RightButton) {
                        edgesID.remove(labelNum)
                    }
            }
            Keys.onPressed: {
                if (activeFocus == true) {
                    if (event.key >= Qt.Key_0 && event.key <= Qt.Key_9) {
                        text_field.text += (event.key - Qt.Key_0)
                    }
                }
            }
            onFocusChanged: {
                if (focus == true) {
                    line.color = Material.accent
                    text_field.text = ""
                } else {
                    line.color = "black"
                    weight = text_field.text == "" ? 1 : text_field.text
                    text_field.text = weight
                }
            }
        }
    }
}
