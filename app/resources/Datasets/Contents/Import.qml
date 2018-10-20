import QtQuick 2.0

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import Dnai.FontAwesome 1.0

import App.Controllers 1.0

Item {

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
                    PropertyChanges{ target: _expendButton; rotation: 0 }
                },
                State{
                    name:"Expended"
                    PropertyChanges{ target: _dataset; height: 160 }
                    PropertyChanges{ target: _expendButton; rotation: 90 }
                }
            ]
            state: "Collapsed"
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
                   NumberAnimation {
                       target: _expendButton
                       property: "rotation"
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
                    NumberAnimation {
                       target: _expendButton
                       property: "rotation"
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

                    MouseArea {
                        id: _expendButton
                        height: 30
                        width: 30
                        anchors.right: parent.right
                        anchors.rightMargin: 30
                        anchors.verticalCenter: parent.verticalCenter
                        propagateComposedEvents: true
                        LabelAwesomeSolid {
                            height: 30
                            width: 30
                            text: "\uf107"
                            color: AppSettings.theme.text.color
                            font.pointSize: 25
                        }
                        onClicked: {
                            if (_dataset.state === "Collapsed") {
                                _dataset.state = "Expended"
                            } else {
                                _dataset.state = "Collapsed"
                            }
                            mouse.accepted = false
                        }
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
                        Column {
                            anchors.fill: parent
                            //PATH
                            Item {
                                id: _path
                                height: 30
                                anchors.left: parent.left
                                anchors.right: parent.right
                                Label {
                                    id: _pathLabel
                                    anchors.left: parent.left
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 15
                                    text: "Path :"
                                }
                                Label {
                                    height: 30
                                    anchors.left: _pathLabel.right
                                    anchors.right: parent.right
                                    anchors.leftMargin: 5
                                    anchors.rightMargin: 15
                                    anchors.verticalCenter: parent.verticalCenter
                                    horizontalAlignment: "AlignRight"
                                    text: model.path
                                }
                            }
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

    //DATASET LIST
    Item {
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: _dropAreaView.top
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
            currentIndex: Editor.datasetHandler.currentDatasetIndex
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 100
            highlightResizeDuration: 50
            delegate: _datasetComponent
            spacing: 15
        }
    }


    //DROP AREA
    Rectangle {
        id: _dropAreaView
        border.width: AppSettings.theme.border.width
        border.color: AppSettings.theme.colors.background.color1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: Editor.datasetHandler.datasetCount > 0 ? 100 : parent.height
        color: AppSettings.theme.colors.background.light

        //DROP LABEL
        Label {
            id: _dropLabel
            anchors.centerIn: parent
            text: "Drop dataset files here (Folder, .csv, .mp4)"
        }

        //DROP AREA LOGIQUE
        DropArea {
            anchors.fill: parent
            onEntered: {
                var isValid = true
                var partial = false
                for (var i in drag.urls)
                {
                    if (!Editor.datasetHandler.validFile(drag.urls[i]))
                    {
                        isValid = false
                    }
                    else {
                        partial = true
                    }
                }
                if (isValid) {
                    _dropAreaView.color = AppSettings.theme.colors.accent.green
                } else if (partial) {
                    _dropLabel.text = "Warning, some files are not supported, they will be ignored"
                    _dropAreaView.color = AppSettings.theme.colors.accent.yellow
                } else {
                    _dropLabel.text = "No supported files found in the selection, all files will be ignored"
                    _dropAreaView.color = AppSettings.theme.colors.accent.red
                }

            }

            onDropped: {
                for (var i in drop.urls)
                {
                    if (Editor.datasetHandler.validFile(drop.urls[i]))
                    {
                        Editor.datasetHandler.createDatasetFromPath(drop.urls[i])
                    }
                }
                _dropLabel.text = "Drop dataset files here (Folder, .csv, .mp4)"
                _dropAreaView.color = AppSettings.theme.colors.background.light
            }

            onExited: {
                _dropLabel.text = "Drop dataset files here (Folder, .csv, .mp4)"
                _dropAreaView.color = AppSettings.theme.colors.background.light
            }
        }
    }
}
