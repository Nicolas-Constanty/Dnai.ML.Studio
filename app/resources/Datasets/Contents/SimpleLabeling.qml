import QtQuick 2.0
import QtQuick.Controls 2.4

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import Dnai.FontAwesome 1.0

import App.Controllers 1.0

import "../../Templates" as T

T.Content {
    id: _contentView
    background: Rectangle {
        id: _background
        color: AppSettings.theme.colors.background.base
    }
    //DATASET DELEGATE COMPONENT
    Component {
        id: _datasetComponent
        Item {
            id: _dataset
            height: 60
            anchors.left: parent.left
            anchors.right: parent.right
            clip: true

            //DATASET STATES
            states: [
                State{
                    name: "Collapsed"
                    PropertyChanges{ target: _dataset; height: 60 }
                },
                State{
                    name:"Expended"
                    PropertyChanges{ target: _dataset; height: _contentView.height - 120 }
                }
            ]
            state: index == Editor.datasetHandler.currentDatasetIndex ? "Expended" : "Collapsed"
            transitions: [
                Transition {
                    from: "Collapsed"
                    to: "Expended"

                   NumberAnimation {
                       target: _dataset
                       property: "height"
                       duration: 150
                       easing.type: Easing.InOutQuad
                   }
                },
                Transition {
                    from: "Expended"
                    to: "Collapsed"
                    NumberAnimation {
                       target: _dataset
                       property: "height"
                       duration: 150
                       easing.type: Easing.InOutQuad
                    }
                }
            ]

            MouseArea {
                anchors.fill: parent
                anchors.topMargin: 1
                anchors.bottomMargin: 1
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                onClicked: {
                    Editor.datasetHandler.currentDatasetIndex = index
                }

                //HEADER
                Item {
                    id: _header
                    height: 60
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Label {
                        text: model.name
                        anchors.left: parent.left
                        anchors.leftMargin: 30
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 12
                    }
                }
                //INFOS
                Item {
                    id: _infos
                    visible: _dataset.state === "Expended"
                    height: 100
                    anchors.top: _header.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 5
                        color:  AppSettings.theme.colors.background.color3
                        border.color: AppSettings.theme.colors.background.color1
                        border.width: 1
//                        ListView {
//                                id: listView
//                                anchors.fill: parent

//                                contentWidth: headerItem.width
//                                flickableDirection: Flickable.HorizontalAndVerticalFlick

//                                header: Row {
//                                    spacing: 1
//                                    function itemAt(index) { return repeater.itemAt(index) }
//                                    Repeater {
//                                        id: repeater
//                                        model: ["Filename", "Label"]
//                                        Label {
//                                            text: modelData
//                                            font.bold: true
//                                            font.pixelSize: 20
//                                            padding: 10
//                                            background: Rectangle { color: "silver" }
//                                        }
//                                    }
//                                }

//                                model: 100
//                                delegate: Column {
//                                    id: delegate
//                                    property int row: index
//                                    Row {
//                                        spacing: 1
//                                        Repeater {
//                                            model: 5
//                                            ItemDelegate {
//                                                property int column: index
//                                                text: qsTr("%1x%2").arg(delegate.row).arg(column)
//                                                width: listView.headerItem.itemAt(column).width
//                                            }
//                                        }
//                                    }
//                                    Rectangle {
//                                        color: "silver"
//                                        width: parent.width
//                                        height: 1
//                                    }
//                                }

//                                ScrollIndicator.horizontal: ScrollIndicator { }
//                                ScrollIndicator.vertical: ScrollIndicator { }
//                            }
                    }
                }

                //BACKGROUND
                Rectangle {
                    id: _background
                    anchors.fill: parent
                    z: -1
                    color: AppSettings.theme.colors.background.base
                    border.color: AppSettings.theme.colors.background.color1
                    border.width: AppSettings.theme.border.width
                    opacity: 0.5
                }
            }
        }
    }
    contentChildren: [
        //DATASET LIST
        Item {
            clip: true
            anchors.fill: parent
            anchors.margins: 15
            ListView {
                id: _listDataset
                anchors.fill: parent
                model: Editor.datasetHandler.datasets
                focus: true
                highlight: Rectangle {
                    color: AppSettings.theme.colors.background.color1
                    radius: 5
                }
                highlightMoveDuration: 100
                currentIndex: Editor.datasetHandler.currentDatasetIndex
                delegate: _datasetComponent
                spacing: 15
                highlightResizeDuration: 50
            }
        }

    ]
}
