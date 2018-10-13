import QtQuick 2.11
import QtQuick.Controls 2.4

import Dnai.Theme 1.0

Item {
    id: _prop
    property string name: "prop"
    property alias value: _value.text

    signal textEdited(string text)

    height: 30
    anchors.left: parent.left
    anchors.right: parent.right
    Label {
        id: _label
        text: name + " :"
        anchors.left: parent.left
        anchors.verticalCenter: _value.verticalCenter
    }

    TextField {
        id: _value
        enableBar: false
        anchors.left: _label.right
        anchors.right: parent.right
        anchors.leftMargin: 5
        onTextEdited: {
            _prop.textEdited(text)
        }
    }
}
