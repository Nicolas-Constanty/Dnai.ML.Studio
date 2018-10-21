import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4

import Dnai.Theme 1.0
import Dnai.Settings 1.0

import "../"

Item {
    id: _tools
    property EditorView editorView
    property list<Item> contents
    property list<Item> parameters
    property ListModel toolsNames

    function next() {
        var nextIndex = control.currentIndex + 1
        if (nextIndex < control.contentChildren.length)
            control.currentIndex = nextIndex
    }

    TabBar {
        id: control
        orientation: ListView.Vertical
        Repeater {
            model: toolsNames
            delegate: TabButton {
                anchors.left: parent.left
                anchors.right: parent.right
                contentItem: Label {
                    text: model.name
                    font.pointSize: 12
                }
            }
        }
    }

    Component {
        id: _contentsComponent
        SwipeView {
            id: _contents
            currentIndex: control.currentIndex
            anchors.fill: parent
            orientation: "Vertical"
            interactive: false
            Component.onCompleted: {
                for (var i in _tools.contents)
                {
                    _contents.addItem(_tools.contents[i])
                }
            }
        }
    }

    Component {
        id: _parametersComponent
        SwipeView {
            id: _parameters
            anchors.fill: parent
            currentIndex: control.currentIndex
            orientation: "Vertical"
            interactive: false
            Component.onCompleted: {
                for (var i in _tools.parameters)
                    _parameters.addItem(_tools.parameters[i])
            }
        }
    }

    Component.onCompleted: {
        editorView.content = _contentsComponent
        editorView.parameters = _parametersComponent
    }
}
