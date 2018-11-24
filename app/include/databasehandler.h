#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QString>
#include "datasetmodel.h"
#include "querymodel.h"
#include "recordfactory.h"

class DatabaseHandler : public QObject {

    Q_OBJECT
    Q_PROPERTY(TableModel* providers READ providers WRITE setProviders NOTIFY providersChanged)
    Q_PROPERTY(TableModel* labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY(TableModel* datasets READ datasets WRITE setDatasets NOTIFY datasetsChanged)
    Q_PROPERTY(TableModel* entries READ entries WRITE setEntries NOTIFY entriesChanged)
    Q_PROPERTY(TableModel* folders READ folders WRITE setFolders NOTIFY foldersChanged)

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

    const RecordFactory &recordFactory() const;

    TableModel *createFolderEntries(int id);

public slots:
    void setProviders(TableModel* providers);

    void setLabels(TableModel* labels);

    void setDatasets(TableModel* datasets);

    void setEntries(TableModel* entries);

    void setFolders(TableModel* folders);

    void updateEntriesRelations(QSqlRecord &);

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
    RecordFactory m_factory;
};

#endif // DATABASEHANDLER_H
