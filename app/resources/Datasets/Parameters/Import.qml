import QtQuick 2.0
import QtQuick.Controls 2.4

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import App.Controllers 1.0
import "../../Templates" as T

T.Parameters {
    anchors.margins: 10
    //HEADER
    title: "Import properties"

    content: [
        //NAME PROPERTY
        T.StringProperty {
            name: "Name"
            visible: Editor.datasetHandler.datasetCount > 0
            value: Editor.datasetHandler.datasetCount > 0 ? Editor.datasetHandler.currentDataset.name : ""
            onTextEdited: {
                Editor.datasetHandler.currentDataset.setName(value)
            }
        },
        //PATH PROPERTY
        T.PathProperty {
            name: "Path"
            visible: Editor.datasetHandler.datasetCount > 0
            value: Editor.datasetHandler.datasetCount > 0 ? Editor.datasetHandler.currentDataset.path : ""
            onTextEdited: {
                Editor.datasetHandler.currentDataset.setPath(value)
            }
        }
    ]
}
