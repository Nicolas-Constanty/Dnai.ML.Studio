import QtQuick 2.0
import QtQuick.Controls 2.4

import Dnai.Theme 1.0

Item {
    property string name: "prop"
    property alias value: _value.text

    height: 30
    Label {
        id: _label
        text: name + " :"
        anchors.left: parent.left
        anchors.leftMargin: 10
    }

    TextField {
        id: _value
        enableBar: false
        anchors.left: _label.right
        anchors.right: parent.right
        anchors.leftMargin: 5
    }
}
