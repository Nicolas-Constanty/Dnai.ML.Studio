#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QString>
#include "datasetmodel.h"
#include "recordfactory.h"

enum FolderStatus : int {
    NOT_LOADED = 1,
    LOADED,
    SYNCHRONISED
};

class DatabaseHandler : public QObject {

    Q_OBJECT
    Q_PROPERTY(TableModel* providers READ providers WRITE setProviders NOTIFY providersChanged)
    Q_PROPERTY(TableModel* labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY(TableModel* datasets READ datasets WRITE setDatasets NOTIFY datasetsChanged)
    Q_PROPERTY(TableModel* entries READ entries WRITE setEntries NOTIFY entriesChanged)
    Q_PROPERTY(TableModel* folders READ folders WRITE setFolders NOTIFY foldersChanged)
    Q_PROPERTY(TableModel* folderStatus READ folderStatus WRITE setFolderStatus NOTIFY folderStatusChanged)

public:
    DatabaseHandler(QObject *parent = nullptr);
    void initDatabases(const QString &dbpath);

    TableModel* providers() const;

    TableModel* labels() const;

    TableModel* datasets() const;

    TableModel* entries() const;

    TableModel* folders() const;

    TableModel* folderStatus() const;

    const RecordFactory &recordFactory() const;

    TableModel *createFolderEntries(int id);

    TableModel *createFolderEntries();


    Q_INVOKABLE void setLabelNames(const QStringList &labelNames);
    Q_INVOKABLE void removeLabel(const int id);

public slots:
    void setProviders(TableModel* providers);

    void setLabels(TableModel* labels);

    void setDatasets(TableModel* datasets);

    void setEntries(TableModel* entries);

    void setFolders(TableModel* folders);

    void setFolderStatus(TableModel* folderStatus);

    void updateEntriesRelations(QSqlRecord &);


signals:
    void providersChanged(TableModel* providers);

    void labelsChanged(TableModel* labels);

    void datasetsChanged(TableModel* datasets);

    void entriesChanged(TableModel* entries);

    void foldersChanged(TableModel* folders);


    void folderStatusChanged(TableModel* folderStatus);

private:
    void initProvider();
    void initLabels();
    void initEntry();
    void initDatasets();
    void initFolders();
    void initFolderStatus();

    TableModel *m_labels;
    TableModel *m_providers;
    TableModel *m_entries;
    TableModel *m_datasets;
    TableModel *m_folders;
    TableModel *m_folderStatus;
    RecordFactory m_factory;
    QString m_dbConnectionName;
};

#endif // DATABASEHANDLER_H
