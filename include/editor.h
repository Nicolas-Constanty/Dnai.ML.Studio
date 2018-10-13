#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include "datasethandler.h"

class Editor : public QObject
{
	Q_OBJECT
    Q_PROPERTY(DatasetHandler *datasetHandler READ datasetHandler CONSTANT)

public:
	explicit Editor(QObject *parent = nullptr);

    DatasetHandler *datasetHandler();

private:
    DatasetHandler m_datasetHandler;
};

#endif // EDITOR_H
