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
    function next() {
        var nextIndex = tabBar.currentIndex + 1
        if (nextIndex < tabBar.contentChildren.length)
            tabBar.currentIndex = nextIndex
    }

    function prev() {
        var nextIndex = tabBar.currentIndex - 1
        if (nextIndex >= 0)
            tabBar.currentIndex = nextIndex
    }

    header: Row {
        TabBar {
            id: tabBar
            currentIndex: swipeView.currentIndex
            property var arr: [null, _dataset, null, null, null, null]
            onCurrentIndexChanged: {
                if (arr[currentIndex] !== null)
                    arr[currentIndex].load = true
            }

            TabButton {
                text: qsTr("Project")
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
            load: true
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

    Rectangle {
        id: _progressBarPanel
        width: parent.width
        anchors.bottom: parent.bottom
        height: 20
        color: AppSettings.theme.colors.background.light
        Label {
            id: _jobName
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            text: Editor.taskManager.jobName
        }
        Label {
            id: _jobInfo
            anchors.right: _progressBar.left
            anchors.left: _jobName.right
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            horizontalAlignment: Text.AlignRight
            anchors.verticalCenter: parent.verticalCenter
            text: Editor.taskManager.progressText
        }

        ProgressBar {
            id: _progressBar
            value: Editor.taskManager.progress
            padding: 2
            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            indeterminate: Editor.taskManager.indeterminate

            contentItem: Item {
                implicitWidth: 200
                implicitHeight: 4

                Rectangle {
                    width: _progressBar.visualPosition * parent.width
                    height: parent.height
                    color: AppSettings.theme.colors.accent.green
                }
            }
        }
        //STATES
        states: [
            State{
                name: "Collapsed"
                PropertyChanges{ target: _progressBarPanel; anchors.bottomMargin: - 20 }
                PropertyChanges{ target: _shadowProgressBar; opacity: 0 }
            },
            State{
                name:"Expended"
                PropertyChanges{ target: _progressBarPanel; anchors.bottomMargin: 0 }
                PropertyChanges{ target: _shadowProgressBar; opacity: 1 }
            }
        ]
        state: Editor.taskManager.jobRunning ? "Expended" : "Collapsed"
        transitions: [
            Transition {
                from: "Collapsed"
                to: "Expended"

               NumberAnimation {
                   target: _progressBarPanel
                   property: "height"
                   duration: 150
                   easing.type: Easing.InOutQuad
               }
               NumberAnimation {
                   target: _shadowProgressBar
                   property: "anchors.bottomMargin"
                   duration: 150
                   easing.type: Easing.InOutQuad
               }
            },
            Transition {
                from: "Expended"
                to: "Collapsed"
                NumberAnimation {
                   target: _progressBarPanel
                   property: "anchors.bottomMargin"
                   duration: 150
                   easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                   target: _shadowProgressBar
                   property: "opacity"
                   duration: 150
                   easing.type: Easing.InOutQuad
                }
            }
        ]
    }

    DropShadow {
        id: _shadowProgressBar
        anchors.fill: _progressBarPanel
        verticalOffset: -3
        radius: 8.0
        samples: 17
        color: "#40000000"
        source: _progressBarPanel
    }
}
