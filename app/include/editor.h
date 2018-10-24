#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include "datasethandler.h"
#include "apihandler.h"
#include "project.h"

class Editor : public QObject
{
	Q_OBJECT
    Q_PROPERTY(DatasetHandler *datasetHandler READ datasetHandler CONSTANT)
    Q_PROPERTY(ApiHandler *apiHandler READ apiHandler CONSTANT)
    Q_PROPERTY(Project *project READ project WRITE setProject NOTIFY projectChanged)
public:
	explicit Editor(QObject *parent = nullptr);

    DatasetHandler *datasetHandler();
    ApiHandler *apiHandler();

	static Editor &instance();

    Project *project() const;

    Q_INVOKABLE bool loadProject(const QString &path);
    Q_INVOKABLE void saveProject();

public slots:
    void setProject(Project * project);

signals:
    void projectChanged(Project * project);

private:
    DatasetHandler m_datasetHandler;
    ApiHandler m_apiHandler;
    Project *m_project;
	static Editor *m_instance;

};

#endif // EDITOR_H
