import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQml 2.12
//import custom 1.0

DropArea {
    id: root
    property var modelID
    property var vertexDummy
    property bool selected: false

    function reset() {
        root.selected = false
        background.border.color = "black"
        modelID.runDijkstra(-1)
        modelID.runDijkstra(-1)
    }

    width: 40; height: width
    x: 30
    y: 40

    Repeater {
        id: edgeMaker
        model: edges
        delegate: EdgeShape { Component.onCompleted: {
                edgesID = edgeMaker.model
                fromX = root.width/2; toX = vertexX
                fromY = root.width/2; toY = vertexY
            }}
    }

    EdgeShape {
        id: selector
        textVisible: false
        opacity: 0
        fromX: root.width/2; fromY: root.width/2;
        //toX: other.mouseX; toY: other.mouseY
    }

    Rectangle {
        id: dragger
        height: 1
        width: 1
        x: root.width/2
        y: root.width/2
        Drag.active: other.drag.active
        Drag.keys: ["edges"]
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: isOnPath ? Material.accent : Material.primary
        border { color: "black"; width: 2 }
        radius: width/2

        Label {
            id: text
            Material.foreground: "white"
            text: labelAlph
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    property var dragParentID
    onEntered: function(drag) {
            background.border.color = Material.accent
            dragParentID = drag.source
        }
    onExited: {
        background.border.color = "black"
    }
    keys: ["edges"]
    onDropped: {
        dragParentID.parent.append(labelNum, root.x + root.width/2, root.y + root.width/2)
    }

    property real vertexX
    property real vertexY
    function append(from_label, from_x, from_y) {
        vertexX = from_x - root.x
        vertexY = from_y - root.y
        edges.append(vertexDummy.createObject(null, {}), from_label, 1)
    }


    MouseArea {
        id: other
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true
        onEntered: background.border.color = Material.accent
        onExited: background.border.color = !selected ? "black" : Material.accent
        hoverEnabled: true
        drag { target: dragger }

        onPositionChanged: {
            if (pressed) {
                var point =  mapToItem (parent, mouse.x, mouse.y);
                selector.toX = point.x
                selector.toY = point.y
                dragger.x = point.x
                dragger.y = point.y
            }
        }
        onPressed: {
                if (pressedButtons & Qt.LeftButton) {
                    selector.opacity = 1
                } else if (pressedButtons & Qt.RightButton) {
                    modelID.remove(index)
                }
        }
        onReleased: {
            selector.opacity = 0
            selector.toX = selector.fromX
            selector.toY = selector.fromY
            dragger.Drag.drop()
            dragger.x = width/2
            dragger.y = width/2
        }
        onClicked: {
            selected = !selected
            modelID.runDijkstra(labelNum)
        }
    }
}
