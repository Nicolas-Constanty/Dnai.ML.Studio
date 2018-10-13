#include "editor.h"

Editor::Editor(QObject* parent) : QObject(parent)
{
}

DatasetHandler* Editor::datasetHandler()
{
    return &m_datasetHandler;
}
