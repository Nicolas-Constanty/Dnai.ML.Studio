import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.3
import QtQuick.Dialogs 1.3

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import Dnai.FontAwesome 1.0

import App.Controllers 1.0

import "../../Templates" as T

T.Content {
    background: Rectangle {
        id: _background
        color: AppSettings.theme.colors.background.base
    }

    header: Item {
        id: _content
        height: 80

        TextField {
            id: _name
            property bool edit: false
            padding: 0
            font.pointSize: 18
            text: "Project n°1"
            onAccepted: {
                _nameDisplay.text = text
                _name.edit = false
            }
            visible: _name.edit
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: "AlignHCenter"
            anchors.verticalCenter: parent.verticalCenter
        }
        Label {
            id: _nameDisplay
            font.pointSize: 18
            text: "Default project"
            visible: !_name.edit
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    _name.edit = true
                    _name.forceActiveFocus()
                }
            }
        }
        TextAwesomeSolid {
            id: _nameIcon
            text: "\uf304"
            visible: !_name.edit
            font.pointSize: 14
            color: AppSettings.theme.text.color
            anchors.left: _nameDisplay.right
            anchors.verticalCenter: _nameDisplay.verticalCenter
            anchors.leftMargin: 20
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    _name.edit = true
                    _name.forceActiveFocus()
                }
            }
        }
    }

    contentChildren: [
        Column {
            anchors.fill: parent
            anchors.topMargin: 50
            anchors.leftMargin: 200
            anchors.rightMargin: 200
            spacing: 15

            Label {
                text: "Project folder :"
            }
            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 40
                TextField {
                    id: _path
                    enabled: false
                    anchors.left: parent.left
                    anchors.right: _selectPath.left
                    anchors.rightMargin: 10
                    inputMethodHints: Qt.ImhUrlCharactersOnly
                }
                Button {
                    id: _selectPath
                    anchors.right: parent.right
                    width: 100
                    text: "Select a folder"
                    onClicked: {
                        _selectFolder.open()
                    }
                }
                FileDialog {
                    id: _selectFolder
                    title: "Choose a directory"
                    folder: shortcuts["documents"]
                    selectFolder: true
                    selectExisting: true
                    onAccepted: {
                        _path.text = _selectFolder.fileUrl
                    }
                }

            }
            Label {
                text: "Description :"
            }
            TextField {
                id: _value
                anchors.left: parent.left
                anchors.right: parent.right
                height: 80
                verticalAlignment: "AlignTop"
                horizontalAlignment: "AlignLeft"
                wrapMode: TextInput.WordWrap
                placeholderText: "Write your project description here..."
                background: Rectangle {
                    color: AppSettings.theme.colors.background.dark
                    border.width: 1
                    border.color: AppSettings.theme.colors.background.color3
                }
                onAccepted: {
                    _value.clear()
                }
            }
            Item {
                id: _save
                height: 80
                width: 200
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    id: control
                    width: 200
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    contentItem: Item {
                        Row {
                            height: 20
                            spacing: 10
                            anchors.centerIn: parent
                            Label {
                                text: "Save"
                                font.pointSize: 12
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            LabelAwesomeSolid {
                                text: "\uf0c7"
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
                    onClicked: {
                    }
                }
            }
        }
    ]
}
