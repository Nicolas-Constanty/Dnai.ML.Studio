import QtQuick 2.0

import Dnai.Theme 1.0
import Dnai.Settings 1.0

import App.Controllers 1.0

Item {

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
            }

            onExited: {
                _dropLabel.text = "Drop dataset files here (Folder, .csv, .mp4)"
                _dropAreaView.color = AppSettings.theme.colors.background.light
            }
        }
    }
}
