import QtQuick 2.0

import "../Templates" as T
import "Contents" as Content
import "Parameters" as Parameters

T.Tools {
    id: _tools
    toolsNames: ListModel {
        ListElement {
            name: "Import"
        }
        ListElement {
            name: "Simple labeling"
        }
    }

    contents: [
        Content.Import{
            nextButton: T.NextButton {
                onClicked: {
                    _tools.next()
                }
            }
            prevButton: T.PrevButton {
                onClicked: {
                    _mainWindow.prev()
                }
            }
        },
        Content.SimpleLabeling {
            nextButton: T.NextButton {
                onClicked: {
                    _mainWindow.next()
                }
            }
            prevButton: T.PrevButton {
                onClicked: {
                    _tools.prev()
                }
            }
        }
    ]

    parameters: [
        Parameters.Import{},
        Parameters.SimpleLabeling{}
    ]
}
