import QtQuick 2.0
import QtQuick.Controls 2.4

import Dnai.Theme 1.0

import "../"

Item {
    id: _tools
    property EditorView editorView
    property list<Item> contents
    property list<Item> parameters
    property ListModel toolsNames


    TabBar {
        orientation: ListView.Vertical
        Repeater {
            model: toolsNames
            delegate: TabButton {
                text: model.name
            }
        }
    }

    Component {
        id: _contentsComponent
        SwipeView {
            id: _contents
            anchors.fill: parent

            Component.onCompleted: {
                for (var i in _tools.contents)
                    _contents.addItem(_tools.contents[i])
            }
        }
    }

    Component {
        id: _parametersComponent
        SwipeView {
            id: _parameters
            anchors.fill: parent

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
