import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQml 2.12
import custom 1.0

ApplicationWindow {
    visible: true
    width: 720
    height: 480
    title: qsTr("Graphy (REII 211 Practical 2 - Luke Nel - 31595529)")

    AdjacencyListQ {
        id: data
        vertices: [
            /*VertexListQ { onIsOnPathChanged: console.log("Yay!"); label:0; edges: [ VertexQ{label: 1; weight: 10}, VertexQ{label: 3; weight: 8} ]}
            VertexListQ { label:1; edges: [ VertexQ{label: 3; weight: 3}, VertexQ{label: 2; weight: 15} ]},
            VertexListQ { label:2; edges: [ VertexQ{label: 3; weight: 3}, VertexQ{label: 0; weight: 15} ]},
            VertexListQ { label:3; edges: [ VertexQ{label: 1; weight: 3}, VertexQ{label: 2; weight: 15} ]}*/
        ]
        onIsBadChanged: background.color = isBad ? Material.accent : Material.background
        onTotalCostChanged: footerText.visible = totalCost == 0 ? false : true
    }
    Component { id: rootListDummy; EdgeListQ {} }
    Component { id: rootVertexDummy; EdgeQ {} }

    Row {
        id: rootview
        anchors.fill: parent
        property int slideDuration: 500
        property int headerHeight: 75
        property int footerHeight: headerHeight
        property color hfColor: Material.foreground

        Column {
            id: listview
            height: parent.height
            width: 0

            Rectangle {
                height: rootview.headerHeight
                width: parent.width
                color: rootview.hfColor
            }
            Rectangle {
                height: parent.height - rootview.headerHeight - rootview.footerHeight
                width: parent.width

                ListView {
                    id: view
                    anchors.fill: parent
                    spacing: 2
                    anchors.margins: { left: 30; right: 10; top: 30; bottom: 10 }
                    displayMarginBeginning: 30

                    model: data
                    delegate: VertexDelegate {Component.onCompleted: {width = listview.width}/*modelID = view.model; dummyID = vertexDummy}*/}
                    focus: true
                }
            }
            Rectangle {
                height: rootview.footerHeight
                width: parent.width
                color: rootview.hfColor
            }

            transitions: Transition {
                    NumberAnimation { properties: "width"; easing.type: Easing.InOutQuad; duration: rootview.slideDuration }
            }
        }

        Column {
            id: treeview
            height: parent.height
            width: parent.width

            states: State {
                name: "debugOn";
                PropertyChanges { target: treeview; width: rootview.width/2 }
                PropertyChanges { target: listview; width: rootview.width/2 }
            }

            transitions: Transition {
                    NumberAnimation { properties: "width"; easing.type: Easing.InOutQuad; duration: rootview.slideDuration }
            }

            Rectangle {
                id: header
                height: rootview.headerHeight
                width: parent.width
                color: rootview.hfColor

                /*Column {
                    id: debugBlock
                    visible: data.debugIsOn
                    width: 80
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    Label {
                        color: Material.background
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "List View"
                        renderType: Text.NativeRendering
                    }
                    Switch {
                        id: debugSwitch
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }*/
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 20
                    Label {
                        color: Material.background
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Undirected Graph"
                    }
                    Switch {
                        anchors.horizontalCenter: parent.horizontalCenter
                        onPositionChanged: data.isUndirected = !data.isUndirected
                    }
                }
                Button {
                    anchors.right: parent.right
                    anchors.rightMargin: 50
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Help"
                    onClicked: overlay.state = ""
                }
            }

            Rectangle {
                id: background
                height: parent.height - rootview.headerHeight - rootview.footerHeight
                width: parent.width
                color: Material.background

                MouseArea {
                    id: area
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
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
                height: rootview.footerHeight
                color: rootview.hfColor
                Label {
                    color: Material.background
                    id: footerText
                    visible: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Total cost: " + data.totalCost
                }

                MouseArea {
                    id: debugSwitch
                    enabled: data.debugIsOn
                    property bool toggle: false
                    width: 40
                    height: 40
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    onClicked: {
                        if (toggle == false) {
                            treeview.state = "debugOn"
                            toggle = true
                        } else if (toggle == true) {
                            treeview.state = ""
                            toggle = false
                        }
                    }
                }
            }
        }
    }

    Item {
        id: overlay
        width: parent.width
        height: parent.height

        Rectangle {
            anchors.fill: parent
            color: Material.foreground
            opacity: 0.95
        }

        TextArea {
            color: Material.background
            anchors.centerIn: parent
            text: '1. Click anywhere to create a vertex.\n' +
                  '2. Drag between vertices to create an edge.\n' +
                  '3. Click on an edge label to edit its weight.\n' +
                  '4. Select two vertices to find the shortest path.\n' +
                  '5. Deselect one vertex before selecting another.\n' +
                  '6. Right-click a vertex or edge label to delete it.\n' +
                  '7. Have fun!\n'
        }

        MouseArea {
            antialiasing: true
            anchors.fill: parent
        }

        Button {
            id: okayButton
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            width: 100
            height: 50
            text: "Okay!"
            onClicked: overlay.state = "retracted"
            Material.foreground: Material.primary
        }
        states: State {
            name: "retracted";
            PropertyChanges { target: overlay; y: -parent.height; visible: false }
        }

        transitions: Transition {
                from: ""; to: "retracted"; reversible: true
                SequentialAnimation {
                    NumberAnimation { properties: "y"; easing.type: Easing.InOutQuad; duration: 500 }
                    PropertyAnimation { properties: "visible" }
                }
            }
    }
}
