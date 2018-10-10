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

            MouseArea {
                anchors.fill: parent
                anchors.topMargin: 1
                anchors.bottomMargin: 1
                anchors.leftMargin: 5
                anchors.rightMargin: 5
                onClicked: {
                    _listDataset.currentIndex = index
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
            delegate: _datasetComponent
            spacing: 15
        }
    }

}
