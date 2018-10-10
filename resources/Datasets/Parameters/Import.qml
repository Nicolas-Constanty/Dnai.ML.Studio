import QtQuick 2.0

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import App.Controllers 1.0
import "../../Templates" as T

Item {
    anchors.margins: 10
    //HEADER
    Rectangle {
        id: _header
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        height: 40
        color: AppSettings.theme.colors.background.base
        Label {
            anchors.centerIn: parent
            text: "Properties"
            font.pointSize: 12
        }
        Rectangle {
            height: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: AppSettings.theme["colors"]["background"]["color1"]
        }
    }
    Column {
        visible: Editor.datasetHandler.datasetCount > 0
        id: _properties
        anchors.margins: 10
        anchors.top: _header.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        spacing: 10

        //NAME PROPERTY
        T.StringProperty {
            name: "Name"
            value: Editor.datasetHandler.datasetCount > 0 ? Editor.datasetHandler.currentDataset.name : ""
            onValueChanged: {
                Editor.datasetHandler.currentDataset.setName(value)
            }
        }
        //PATH PROPERTY
        T.PathProperty {
            name: "Path"
            value: Editor.datasetHandler.datasetCount > 0 ? Editor.datasetHandler.currentDataset.path : ""
            onValueChanged: {
                Editor.datasetHandler.currentDataset.setPath(value)
            }
        }
    }
}
