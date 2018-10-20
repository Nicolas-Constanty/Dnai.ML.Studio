import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.3
import QtGraphicalEffects 1.0

import "Datasets/"
import "Settings/"

import Dnai.Settings 1.0
import Dnai.Theme 1.0
import Dnai.FontAwesome 1.0
import App.Controllers 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    title: qsTr("Ml Studio")
    id : _mainWindow

    color: AppSettings.theme.colors.background.dark

    header: Row {
//        Button {
//            id: control
//            height: parent.height
//            width: height
//            contentItem: LabelAwesomeSolid {
//                text: "\uf2f6"
//                font.pointSize: 16
//                horizontalAlignment: "AlignHCenter"
//                verticalAlignment: "AlignVCenter"
//                color: AppSettings.theme.text.color
//            }
//            background: Rectangle {
//                color: Color.blend(control.checked ? AppSettings.theme.colors.background.color1 : control.hovered ? AppSettings.theme.colors.background.color3 : AppSettings.theme.colors.background.color2,
//                                                     AppSettings.theme.colors.background.base, control.down ? 0.5 : 0.0)
//            }
//            onClicked: {
//                _authView.open()
//                _authView.x = (_mainWindow.width - _authView.width) / 2
//                _authView.y = (_mainWindow.height - _authView.width + _mainWindow.header.height) / 2
//            }
//        }
        TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
//            width: parent.width - parent.height
            TabButton {
                text: qsTr("Settings")
            }
            TabButton {
                text: qsTr("Dataset")
            }
            TabButton {
                text: qsTr("Hyper Parameters")
            }
            TabButton {
                text: qsTr("Neural Network")
            }
            TabButton {
                text: qsTr("Training")
            }
            TabButton {
                text: qsTr("Result")
            }

            background: Rectangle {
                color: AppSettings.theme.colors.background.color2
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        interactive: false
        EditorView {
            id: _settings
            tools: SettingsTools {
                editorView: _settings
            }
            enableParameters: false
        }
        EditorView {
            id: _dataset
            tools: DatasetTools {
                editorView: _dataset
            }
        }
        EditorView {
            enableTools: false
            enableParameters: false
        }
        EditorView {
            enableTools: false
        }
        EditorView {
            enableTools: false
            enableParameters: false
        }
        EditorView {
            enableTools: false
            enableParameters: false
        }
    }

    Rectangle {
        id: _spitter
        height: 2
        anchors.left: parent.left
        anchors.right: parent.right
        color: AppSettings.theme.colors.background.color1
    }
    DropShadow {
        anchors.fill: _spitter
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: _spitter
    }

    AuthView {
        id: _authView
    }
}
