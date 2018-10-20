import QtQuick 2.11
import QtQuick.Controls 2.4

import Dnai.Theme 1.0
import Dnai.Settings 1.0

Item {
    id: _prop
    property string name: "prop"
    property alias value: _value.text

    signal recordTag(string text)

    height: childrenRect.height
    anchors.left: parent.left
    anchors.right: parent.right
    Label {
        height: 30
        id: _label
        text: name + " :"
        anchors.left: parent.left
    }

    Rectangle {
        height: 80
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: _label.bottom
        anchors.topMargin: 5
        TextField {
            id: _value
            anchors.fill: parent
            verticalAlignment: "AlignTop"
            horizontalAlignment: "AlignLeft"
            wrapMode: TextInput.WordWrap
            placeholderText: "Create new tags, separate tags with a space..."
            background: Rectangle {
                color: AppSettings.theme.colors.background.color2
                border.width: 1
                border.color: AppSettings.theme.colors.background.color3
            }
            onAccepted: {
                _prop.recordTag(text)
                _value.clear()
            }
        }
    }
}
