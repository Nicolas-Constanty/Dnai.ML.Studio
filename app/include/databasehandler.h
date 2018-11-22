#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QString>
#include "datasetmodel.h"

class DatabaseHandler : public QObject {

    Q_OBJECT
    Q_PROPERTY(TableModel* providers READ providers WRITE setProviders NOTIFY providersChanged)
    Q_PROPERTY(TableModel* labels READ labels WRITE setLabels NOTIFY labelsChanged CONSTANT)
    Q_PROPERTY(TableModel* datasets READ datasets WRITE setDatasets NOTIFY datasetsChanged CONSTANT)
    Q_PROPERTY(TableModel* entries READ entries WRITE setEntries NOTIFY entriesChanged CONSTANT)
    Q_PROPERTY(TableModel* folders READ folders WRITE setFolders NOTIFY foldersChanged CONSTANT)

public:
    DatabaseHandler(QObject *parent = nullptr);
    void initDatabases(const QString &dbpath);

    TableModel* providers() const;

    TableModel* labels() const;

    TableModel* datasets() const;

    TableModel* entries() const;

    Q_INVOKABLE void setLabelNames(const QStringList &labelNames);
    Q_INVOKABLE void removeLabel(const int id);

    TableModel* folders() const;

public slots:
    void setProviders(TableModel* providers);

    void setLabels(TableModel* labels);

    void setDatasets(TableModel* datasets);

    void setEntries(TableModel* entries);

    void setFolders(TableModel* folders);

signals:
    void providersChanged(TableModel* providers);

    void labelsChanged(TableModel* labels);

    void datasetsChanged(TableModel* datasets);

    void entriesChanged(TableModel* entries);

    void foldersChanged(TableModel* folders);


private:
    void initProvider();
    void initLabels();
    void initEntry();
    void initDatasets();
    void initFolders();

    TableModel *m_labels;
    TableModel *m_providers;
    TableModel *m_entries;
    TableModel *m_datasets;
    TableModel *m_folders;
};

#endif // DATABASEHANDLER_H
