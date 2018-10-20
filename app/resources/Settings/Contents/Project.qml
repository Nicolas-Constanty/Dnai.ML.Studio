import QtQuick 2.11

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import Dnai.FontAwesome 1.0

Item {
    Rectangle {
        id: _background
        anchors.fill: parent
        color: AppSettings.theme.colors.background.base
    }
    Item {
        id: _content
        anchors.fill: parent
        anchors.margins: 15

        TextField {
            id: _name
            property bool edit: false
            padding: 0
            font.pointSize: 30
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
        }
        Label {
            id: _nameDisplay
            font.pointSize: 30
            text: "Default project"
            visible: !_name.edit
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
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
            font.pointSize: 16
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
//        Row {
//            id: _nameProp
//            anchors.top: parent.top
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.topMargin: 30
//            height: 40
//            spacing: 20
//            property bool edit: false



//        }
    }
}
