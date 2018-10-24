import QtQuick 2.0
import QtQuick.Controls 2.4
import Dnai.Theme 1.0
import Dnai.Settings 1.0
import "../../Templates" as T
import QtQuick.Layouts 1.11 as L

import App.Controllers 1.0
import Dnai.FontAwesome 1.0

T.Parameters {
    anchors.margins: 10
    //HEADER
    title: "Labels properties"

    content: [
        T.TextAreaProperty {
            name: "Tags"
            onRecordTag: {
                var tlist = text.trim().split(" ")
                Editor.datasetHandler.setLabelNames(tlist)
            }
        },
        Grid {
            columns: 4
            spacing: 5
            height: 200
            anchors.left: parent.left
            anchors.right: parent.right
            Repeater {
                model: Editor.datasetHandler.labelNames
                delegate: Rectangle {
                    width: _r.width + 10
                    height: 18
                    Row {
                        id: _r
                        height: 18
                        anchors.centerIn: parent
                        spacing: 4
                        Label {
                            id: _tagLabel
                            text: model.name
                            width: 35
                        }
                        MouseArea {
                            id: _deleteButton
                            height: 18
                            width: 18
                            hoverEnabled: true
                            LabelAwesomeSolid {
                                anchors.centerIn: parent
                                text: "\uf00d"
                                color: AppSettings.theme.text.color

                            }
                            onClicked: {
                                Editor.datasetHandler.removeLabel(model.id)
                            }
                        }
                    }

                    radius: 4
                    color: _deleteButton.containsMouse ? AppSettings.theme.colors.accent.red : AppSettings.theme.colors.accent.green
                }
            }
        }
    ]
}
