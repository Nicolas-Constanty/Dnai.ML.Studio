import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3

import Dnai.Theme 1.0
import Dnai.FontAwesome 1.0
import Dnai.Settings 1.0

Item {
    property string name: "prop"
    property alias value: _value.text

    height: childrenRect.height
    anchors.left: parent.left
    anchors.right: parent.right
    Label {
        id: _label
        height: 30
        text: name + " :"
        anchors.left: parent.left
        anchors.verticalCenter: _value.verticalCenter
    }

    Label {
        id: _value
        height: 30
        anchors.left: _label.right
        anchors.right: parent.right
        anchors.leftMargin: 5
        horizontalAlignment: "AlignRight"
    }

    Button {
        id: _openFolder
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _value.bottom
        anchors.topMargin: 5
        contentItem: Item {
            anchors.fill: parent
            Row {
                height: 40
                spacing: 5
                anchors.centerIn: parent
                Label {
                    text: "Select a folder"
                    anchors.verticalCenter: parent.verticalCenter
                }
                LabelAwesomeSolid {
                    text: "\uf07c"
                    color: AppSettings.theme.colors.accent.yellow
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 20
                }
            }
        }
        onClicked: {
            _selectFolder.open()
        }
    }

    Button {
        id: _openFile
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _openFolder.bottom
        anchors.topMargin: 5
        contentItem: Item {
            anchors.fill: parent
            Row {
                height: 40
                spacing: 5
                anchors.centerIn: parent
                Label {
                    text: "Select a File"
                    anchors.verticalCenter: parent.verticalCenter
                }
                LabelAwesomeSolid {
                    text: "\uf07c"
                    color: AppSettings.theme.colors.accent.yellow
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 20
                }
            }
        }
        onClicked: {
            _selectFile.open()
        }
    }

    FileDialog {
        id: _selectFolder
        title: "Choose a directory"
        folder: shortcuts["documents"]
        selectFolder: true
        selectExisting: true
        onAccepted: {
            _value.text = _selectFolder.fileUrl
        }
    }

    FileDialog {
        id: _selectFile
        title: "Choose a file"
        folder: shortcuts["documents"]
        selectFolder: false
        selectExisting: true
        selectMultiple: false
        nameFilters: [ "CSS files (*.csv)", "Video files (*.mp4)", "All files (*.csv *.mp4)" ]
        onAccepted: {
            _value.text = _selectFile.fileUrl
        }
    }
}
