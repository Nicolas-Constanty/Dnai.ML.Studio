import QtQuick 2.0
import QtQuick.Controls 2.4
import Dnai.Theme 1.0
import Dnai.Settings 1.0

Item {
    anchors.margins: 10
    //HEADER
    Item {
        id: _header
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        height: 60
        Label {
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
}
