import QtQuick 2.11
import QtQuick.Controls 2.4

import "Datasets/"

import Dnai.Settings 1.0
import Dnai.Theme 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    title: qsTr("Ml Studio")

    header: TabBar {
        id: tabBar

        currentIndex: swipeView.currentIndex

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
            text: qsTr("Result")
        }

        background: Rectangle {
            color: AppSettings.theme.colors.background.color1
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        EditorView {
            id: _dataset
            tools: DatasetTools {
                editorView: _dataset
            }
        }
        EditorView {}
        EditorView {}
        EditorView {}
    }

    Rectangle {
        height: 2
        anchors.left: parent.left
        anchors.right: parent.right
        color: AppSettings.theme.colors.background.color1
    }
}
