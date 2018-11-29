import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.3

import Dnai.Settings 1.0
import Dnai.Theme 1.0
import Dnai.FontAwesome 1.0

import App.Controllers 1.0

Popup {
    id: _control
    width: 400
    height: 230
    modal: true
    padding: 0

    Rectangle {
        anchors.fill: parent
        color: AppSettings.theme.colors.background.light
        border.width: 1
        border.color: AppSettings.theme.colors.background.color1
    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10
        anchors.topMargin: 5
        Item {
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right
            Label {
                anchors.centerIn: parent
                text: "Ml Server connection"
                font.pointSize: 14
            }
        }
        Label {
            text: "ML Server API address :"
        }
        TextField {
            id: _addr
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: "Enter your api address"
            inputMethodHints: Qt.ImhUrlCharactersOnly
        }
        Label {
            text: "Password :"
        }
        TextField {
            id: _password
            anchors.left: parent.left
            anchors.right: parent.right
            placeholderText: "Enter password..."
            echoMode: TextInput.Password
        }
        Button {
            id: control
            width: 200
            height: 30
            anchors.horizontalCenter: parent.horizontalCenter
            contentItem: Item {
                Row {
                    height: 20
                    spacing: 10
                    anchors.centerIn: parent
                    Label {
                        text: "Connect to server"
                        font.pointSize: 12
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    LabelAwesomeSolid {
                        text: "\uf2f6"
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
                Editor.apiHandler.addr = _addr.text
                Editor.apiHandler.connect(_password.text)
                _control.close()
            }
        }
    }
}
