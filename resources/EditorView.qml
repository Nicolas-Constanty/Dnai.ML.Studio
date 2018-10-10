import QtQuick 2.11
import QtQuick.Controls 1.4 as Old
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import Dnai.Theme 1.0
import Dnai.Settings 1.0

Item {
    property bool enableParameters: true
    property bool enableTools: true

    property alias tools: _tools._contentChildren
    property alias parameters: _parameters._contentChildren
    property alias content: _contentView._contentChildren
    property alias contentItem: _contentViewLoader.item
    property alias parametersItem: _parametersLoader .item
    property alias toolsItem: _toolsLoader.item

    Layout.fillHeight: true
    Layout.fillWidth: true
    Old.SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        Rectangle {
            id: _tools
            property alias _contentChildren: _toolsLoader.sourceComponent
            color: AppSettings.theme["colors"]["background"]["base"]
            visible: enableTools
            width: 200
            Layout.maximumWidth: 250
            Layout.minimumWidth: 100
            Loader {
                id: _toolsLoader
                anchors.fill: parent
                active: true
            }

//            Label {
//                text: "Tools"
//                anchors.centerIn: parent
//            }
        }
        Rectangle {
            id: _contentView
            property alias _contentChildren: _contentViewLoader.sourceComponent
            color: AppSettings.theme["colors"]["background"]["base"]
            Layout.minimumWidth: 50
            Layout.fillWidth: true
//            Label {
//                text: "Content"
//                anchors.centerIn: parent
//            }
            Loader {
                id: _contentViewLoader
                anchors.fill: parent
                active: true
            }
        }
        Rectangle {
            id: _parameters
            property alias _contentChildren: _parametersLoader.sourceComponent
            color: AppSettings.theme["colors"]["background"]["base"]
            visible: enableParameters
            width: 300
            Layout.maximumWidth: 300
            Layout.minimumWidth: 200
//            Label {
//                text: "Content Parmeters"
//                anchors.centerIn: parent
//            }
            Loader {
                id: _parametersLoader
                anchors.fill: parent
                active: true
            }
        }
    }
}
