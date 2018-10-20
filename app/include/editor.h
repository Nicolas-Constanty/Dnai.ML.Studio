#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include "datasethandler.h"
#include "apihandler.h"

class Editor : public QObject
{
	Q_OBJECT
    Q_PROPERTY(DatasetHandler *datasetHandler READ datasetHandler CONSTANT)
    Q_PROPERTY(ApiHandler *apiHandler READ apiHandler CONSTANT)

public:
	explicit Editor(QObject *parent = nullptr);

    DatasetHandler *datasetHandler();
    ApiHandler *apiHandler();

	static Editor &instance();

private:
    DatasetHandler m_datasetHandler;
    ApiHandler m_apiHandler;
	static Editor *m_instance;

};

#endif // EDITOR_H
