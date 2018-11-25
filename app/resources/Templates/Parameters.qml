import QtQuick 2.0

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import App.Controllers 1.0

Item {
    id: _parameters
    property alias title: _headerTitle.text
    property list<Item> contents
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
            id: _headerTitle
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
        id: _properties
        anchors.margins: 10
        anchors.top: _header.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        spacing: 10

        Component.onCompleted: {
            _properties.children = _parameters.contents
        }
    }
}
