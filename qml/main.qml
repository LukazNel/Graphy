import QtQuick 2.15
import QtQml 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import custom 1.0

Window {
    visible: true
    width: 720
    height: 480
    title: qsTr("Hello World")

    AdjacencyListQ {
        id: data
        vertices: [
            /*VertexListQ { onIsOnPathChanged: console.log("Yay!"); label:0; edges: [ VertexQ{label: 1; weight: 10}, VertexQ{label: 3; weight: 8} ]}
            VertexListQ { label:1; edges: [ VertexQ{label: 3; weight: 3}, VertexQ{label: 2; weight: 15} ]},
            VertexListQ { label:2; edges: [ VertexQ{label: 3; weight: 3}, VertexQ{label: 0; weight: 15} ]},
            VertexListQ { label:3; edges: [ VertexQ{label: 1; weight: 3}, VertexQ{label: 2; weight: 15} ]}*/
        ]
        onIsBadChanged: background.color = isBad ? "red" : "steelblue"
        onTotalCostChanged: footerText.visible = totalCost == 0 ? false : true
    }
    Component { id: rootListDummy; VertexListQ {} }
    Component { id: rootVertexDummy; VertexQ {} }
    Row {
        anchors.fill: parent
        Rectangle {
            id: listview
            height: parent.height
            width: parent.width / 2
            color: "green"

            ListView {
                id: view
                anchors.fill: parent
                anchors.margins: { left: 30; right: 10; top: 30; bottom: 10 }
                displayMarginBeginning: 30

                model: data
                delegate: VertexDelegate {Component.onCompleted: {width = listview.width}/*modelID = view.model; dummyID = vertexDummy}*/}
                focus: true
            }
        }

        Column {
            id: treeview
            height: parent.height
            width: parent.width / 2
            //color: "steelblue"

            Rectangle {
                id: header
                color: "white"
                width: parent.width
                height: 50
                Column {
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 20
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Undirected Graph"
                    }
                    Switch {
                        anchors.horizontalCenter: parent.horizontalCenter
                        onPositionChanged: data.isUndirected = !data.isUndirected
                    }
                }
            }

            Rectangle {
                id: background
                width: parent.width
                height: parent.height - 100
                color: "steelblue"
                MouseArea {
                    id: area
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onPressed: {
                        area.focus = true
                        if (area.pressedButtons == Qt.LeftButton)
                            data.append(rootListDummy.createObject(null, {}))
                    }
                }

                Repeater {
                    id: tree
                    model: data
                    delegate: VertexShape {
                        Component.onCompleted: { modelID = tree.model;
                            vertexDummy = rootVertexDummy; x = area.mouseX - width/2;
                            y = area.mouseY - width/2; area.onClicked.connect(reset)}}
                }
            }

            Rectangle {
                id: footer
                width: parent.width
                height: 50
                Text {
                    id: footerText
                    visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Total cost: " + data.totalCost
                }
            }
        }
    }

    /*


    Button {
        text: "Press It!"
        highlighted: true
        flat: true
        anchors.right: parent.right

        onClicked: list.makeIt()
    }*/
}
