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

        //TEXT FIELD PROJECT NAME
        TextField {
            id: _name
            property bool edit: false
            padding: 0
            font.pointSize: 18
            text: "Default project"
            onAccepted: {
                Editor.project.name = text
                _name.edit = false
            }
            visible: _name.edit
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: "AlignHCenter"
            anchors.verticalCenter: parent.verticalCenter
        }
        //LABEL FOR DISPLAYING PROJECT NAME
        Label {
            id: _nameDisplay
            font.pointSize: 18
            visible: !_name.edit
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: Editor.project.name
            MouseArea {
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                onClicked: {
                    _name.edit = true
                    _name.forceActiveFocus()
                }
            }
        }
        //ICON FOR EDIT PROJECT NAME
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
                cursorShape: Qt.PointingHandCursor
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

            //PROJECT FOLDER SELECTION
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
                    text: Editor.project.path
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
                        Editor.project.path = _path.text
                    }
                }

            }
            //PROJECT DESCRIPTION
            Label {
                text: "Description :"
            }
            TextField {
                id: _value
                text: Editor.project.description
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
                    Editor.project.description = _value.text
                }
            }
            Item {
                id: _save
                height: 60
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
                        Editor.saveProject()
                    }
                }
            }
            Item {
                id: _open
                height: 30
                width: 200
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    id: _openButton
                    width: 200
                    height: 30
                    anchors.verticalCenter: parent.verticalCenter
                    contentItem: Item {
                        Row {
                            height: 20
                            spacing: 10
                            anchors.centerIn: parent
                            Label {
                                text: "Open"
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
                        color: Color.blend(_openButton.checked || _openButton.highlighted ? AppSettings.theme.colors.background.dark : _openButton.hovered ? "#97cbb0" : AppSettings.theme.colors.accent.green,
                                                                                    AppSettings.theme.colors.background.light, _openButton.down ? 0.5 : 0.0)
                        radius: 5
                    }
                    onClicked: {
                        _selectFile.open()
                    }
                }
                FileDialog {
                    id: _selectFile
                    title: "Choose a project"
                    folder: shortcuts["documents"]
                    selectFolder: false
                    selectExisting: true
                    selectMultiple: false
                    nameFilters: [ "project files (*.mlstd)" ]
                    onAccepted: {
                        if (!Editor.loadProject(_selectFile.fileUrl))
                        {
                            console.warn("Cannot open the projet: ", _selectFile.fileUrl)
                        }
                    }
                }
            }
        }
    ]
}
