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
            property var modelIndex: index

            //DATASET STATES
            states: [
                State{
                    name: "Collapsed"
                    PropertyChanges{ target: _dataset; height: 60 }
                },
                State{
                    name:"Expended"
                    PropertyChanges{ target: _dataset; height: Editor.datasetHandler.folder(_dataset.modelIndex).rowCount() > 15 ? _contentView.height - 120 : Editor.datasetHandler.folder(_dataset.modelIndex).rowCount() * 34 + 90 }
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
                        ListView {
                                id: listView
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 4

                                flickableDirection: Flickable.HorizontalAndVerticalFlick

                                model: Editor.datasetHandler.folder(_dataset.modelIndex)
                                delegate: Rectangle {
                                    id: delegate
                                    property var folder: model
                                    height: 30
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    color: AppSettings.theme.colors.background.light
                                    property int row: index
                                    Label {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 10
                                        text: delegate.folder.name
                                        height: parent.height
                                    }
                                    ComboBox {
                                        textRole: "name"
                                        anchors.verticalCenter: parent.verticalCenter
                                        model: Editor.databaseHandler.labels
                                        anchors.right: _status.left
                                        anchors.rightMargin: 15
                                        visible: Editor.databaseHandler.labels.rowCount() > 1
                                        currentIndex: delegate.folder._labelId
                                    }

                                    T.FolderStatus {
                                        id: _status
                                        status: delegate.folder._folderStatusId
                                        radius: 4
                                        anchors.right: parent.right
                                        anchors.rightMargin: 15
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }

                                ScrollIndicator.horizontal: ScrollIndicator { }
                                ScrollIndicator.vertical: ScrollIndicator { }
                            }
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
                model: Editor.databaseHandler.datasets
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
