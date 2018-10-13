import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.3
import QtGraphicalEffects 1.0

import "Datasets/"

import Dnai.Settings 1.0
import Dnai.Theme 1.0
import Dnai.FontAwesome 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    title: qsTr("Ml Studio")

    color: AppSettings.theme.colors.background.dark

    header: TabBar {
        id: tabBar
//        visible: false
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Dataset")
        }
        TabButton {
            text: qsTr("Hyper Parameters")
        }
        TabButton {
            text: qsTr("Neural Network")
        }
        TabButton {
            text: qsTr("Training")
        }
        TabButton {
            text: qsTr("Result")
        }

        background: Rectangle {
            color: AppSettings.theme.colors.background.color2
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive: false
//        visible: false
        EditorView {
            id: _dataset
            tools: DatasetTools {
                editorView: _dataset
            }
        }
        EditorView {
            enableTools: false
            enableParameters: false
        }
        EditorView {
            enableTools: false
        }
        EditorView {
            enableTools: false
            enableParameters: false
        }
        EditorView {
            enableTools: false
            enableParameters: false
        }
    }

    Rectangle {
        id: _spitter
//        visible: false
        height: 2
        anchors.left: parent.left
        anchors.right: parent.right
        color: AppSettings.theme.colors.background.color1
    }
    DropShadow {
        anchors.fill: _spitter
//        visible: false
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: _spitter
    }


    Item {
        width: 400
        height: 160
        anchors.centerIn: parent
        visible: false

        Rectangle {
            anchors.fill: parent
            color: AppSettings.theme.colors.background.light
            border.width: 1
            border.color: AppSettings.theme.colors.background.color1
        }

        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10
            anchors.topMargin: 5
            Item {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                Label {
                    anchors.centerIn: parent
                    text: "Ml Server connection"
                    font.pointSize: 14
                }
            }
            Label {
                text: "ML Server API address :"
            }
            TextField {
                anchors.left: parent.left
                anchors.right: parent.right
                placeholderText: "Enter your api address"
                inputMethodHints: Qt.ImhUrlCharactersOnly
            }
//            Label {
//                text: "Server port :"
//            }
//            TextField {
//                anchors.left: parent.left
//                anchors.right: parent.right
//                placeholderText: "Enter ml server port"
//            }
            Button {
                id: control
                width: 200
                height: 30
                anchors.horizontalCenter: parent.horizontalCenter
                contentItem: Item {
                    Row {
                        height: 20
                        spacing: 10
                        anchors.centerIn: parent
                        Label {
                            text: "Connect to server"
                            font.pointSize: 12
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        LabelAwesomeSolid {
                            text: "\uf2f6"
                            font.pointSize: 14
                            color: AppSettings.theme.text.color
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
                background: Rectangle {
                    color: Color.blend(control.checked || control.highlighted ? AppSettings.theme.colors.background.dark : control.hovered ? "#97cbb0" : AppSettings.theme.colors.accent.green,
                                                                                AppSettings.theme.colors.background.light, control.down ? 0.5 : 0.0)
                    radius: 5
                }
            }
        }
    }
}
