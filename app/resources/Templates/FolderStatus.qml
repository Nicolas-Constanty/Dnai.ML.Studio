import QtQuick 2.0
import QtQuick.Controls 2.4

import Dnai.Settings 1.0

Rectangle {
    id: rec
    property int status: 1
    width: radius * 2
    height: radius * 2
    Rectangle {
        width: radius * 2
        height: radius * 2
        radius: parent.radius + 3
        color: parent.color
        opacity: parent.opacity / 2
        anchors.centerIn: parent
        Rectangle {
            width: radius * 2
            height: radius * 2
            radius: parent.radius + 3
            color: parent.color
            opacity: parent.opacity / 2
            anchors.centerIn: parent
        }
    }

    function updateColor() {
        if (status == 2)
            rec.color = AppSettings.theme.colors.accent.yellow
        else if (status == 3)
            rec.color = AppSettings.theme.colors.accent.green
        else
            rec.color = AppSettings.theme.colors.accent.red
    }

    onStatusChanged: {
        updateColor()
    }


    Component.onCompleted: {
        updateColor()
    }
}


