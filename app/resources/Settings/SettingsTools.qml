import QtQuick 2.0

import "../Templates" as T
import "Contents" as Content

T.Tools {

    toolsNames: ListModel {
        ListElement {
            name: "Project"
        }
        ListElement {
            name: "Docker Connection"
        }
    }

    contents: [
        Content.Project{},
        Content.Docker{}
    ]
}
