import QtQuick 2.0
import QtQuick.Controls 2.4

import Dnai.Theme 1.0
import Dnai.Settings 1.0
import App.Controllers 1.0
import "../../Templates" as T

T.Parameters {
    property TableModel model: Editor.databaseHandler.datasets
    anchors.margins: 10
    //HEADER
    title: "Import properties"

    content: [
        //NAME PROPERTY
        T.StringProperty {
            name: "Name"
            visible: Editor.databaseHandler.datasets != null && Editor.databaseHandler.datasets.count > 0
            value: visible ? model.data(model.index(Editor.datasetHandler.currentDatasetIndex, 0), model.getRoles("name")) : ""
            onTextEdited: {
                Editor.datasetHandler.currentDataset.setName(value)
            }
        },
        //PATH PROPERTY
        T.PathProperty {
            name: "Path"
            visible: Editor.databaseHandler.datasets != null && Editor.databaseHandler.datasets.count > 0
            value: visible ? model.data(model.index(Editor.datasetHandler.currentDatasetIndex, 0), model.getRoles("path")) : ""
            onTextEdited: {
                Editor.datasetHandler.currentDataset.setPath(value)
            }
        }
    ]
}
