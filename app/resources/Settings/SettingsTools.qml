import QtQuick 2.0

import "../Templates" as T
import "Contents" as Content

T.Tools {
    id: _tools
    toolsNames: ListModel {
        ListElement {
            name: "Save to"
        }
        ListElement {
            name: "Docker Connection"
        }
    }

    contents: [
        Content.Project{
            nextButton: T.NextButton {
                onClicked: {
                    _tools.next()
                }
            }
        },
        Content.Docker{
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
}
