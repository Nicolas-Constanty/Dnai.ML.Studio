import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.3

import Dnai.Settings 1.0
import Dnai.Theme 1.0
import Dnai.FontAwesome 1.0

import App.Controllers 1.0

import "../../Templates" as T

T.Content {
    background: Rectangle {
        color: AppSettings.theme.colors.background.base
    }

    header: Item {
        height: 80
        Label {
            anchors.centerIn: parent
            text: "Ml Server connection"
            font.pointSize: 18
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
            Item {
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
                    }
                }
            }
        }
    ]

    footer: Item {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 200
        Row {
            anchors.centerIn: parent
            height: 128
            spacing: 20
            Image {
                height: 128
                width: 128
                source: "qrc:/resources/Images/Docker.png"
                anchors.verticalCenter: parent.verticalCenter
            }
            Label {
                text: "+"
                font.pointSize: 40
                color: AppSettings.theme.text.color
                anchors.verticalCenter: parent.verticalCenter
            }
            Image {
                height: 128
                width: 128
                source: "qrc:/resources/Images/Keras.png"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
