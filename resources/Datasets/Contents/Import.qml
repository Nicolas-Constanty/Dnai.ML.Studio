import QtQuick 2.0

import Dnai.Theme 1.0
import Dnai.Settings 1.0

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
            anchors.margins: 5

            MouseArea {
                anchors.fill: parent
                anchors.topMargin: 1
                anchors.bottomMargin: 1
                onClicked: {
                    _listDataset.currentIndex = index
                    Editor.datasetHandler.currentDatasetIndex = index
                }

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
                Rectangle {
                    id: _background
                    anchors.fill: parent
                    z: -1
                    color: AppSettings.theme.colors.background.base
                    border.color: AppSettings.theme.border.color
                    border.width: AppSettings.theme.border.width
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
                anchors.left: parent.left
                anchors.right: parent.right
                color: "lightsteelblue"; radius: 5
            }
            delegate: _datasetComponent
            spacing: 15
        }
    }


    //DROP AREA
    Rectangle {
        id: _dropAreaView
        border.width: AppSettings.theme.border.width
        border.color: AppSettings.theme.border.color
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 100
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
