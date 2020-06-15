import QtQuick 2.15
import QtQml 2.15
import custom 1.0

DropArea {
    id: root
    property var modelID
    property var vertexDummy
    property bool selected: false

    function reset() {
        root.selected = false
        highlight.visible = false
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
        anchors.fill: parent
        color: isOnPath ? "green" : "red"
        radius: width/2

        Text {
            id: text
            text: label
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Rectangle {
        id: highlight
        visible: false
        anchors.fill: parent
        radius: width/2
        color: "#00000000"
        border {
            color: "black"
            width: 2
        }
    }

    property var dragParentID
    onEntered: function(drag) {
            highlight.visible = true
            dragParentID = drag.source
        }
    onExited: {
        highlight.visible = false
    }
    keys: ["edges"]
    onDropped: {
        dragParentID.parent.append(label, root.x + root.width/2, root.y + root.width/2)
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
        onEntered: highlight.visible = true
        onExited: !selected ? highlight.visible = false : true
        hoverEnabled: true
        drag {
            target: dragger
        }

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
            //if (selected == true) {
                modelID.runDijkstra(label)
            //} else {
            //    modelID.runDijkstra(-1)
            //}
        }
    }
}
