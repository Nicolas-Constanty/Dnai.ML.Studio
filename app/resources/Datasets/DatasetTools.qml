import QtQuick 2.0

import "../Templates" as T
import "Contents" as Content
import "Parameters" as Parameters

T.Tools {

    toolsNames: ListModel {
        ListElement {
            name: "Import"
        }
        ListElement {
            name: "Simple labeling"
        }
    }

    contents: [
        Content.Import{},
        Content.SimpleLabeling{}
    ]

    parameters: [
        Parameters.Import{},
        Parameters.SimpleLabeling{}
    ]
}
