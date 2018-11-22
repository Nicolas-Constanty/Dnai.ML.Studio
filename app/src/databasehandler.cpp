#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

#include "databasehandler.h"

DatabaseHandler::DatabaseHandler(QObject *parent) : QObject (parent)
{
    m_labels = nullptr;
    m_providers = nullptr;
    m_datasets = nullptr;
    m_entries = nullptr;
    m_folders = nullptr;
}

void DatabaseHandler::initDatabases(const QString &dbpath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("dnai");
    db.setDatabaseName(dbpath);
    db.setUserName("user");
    db.setPassword("dn41");
    if (!db.open()) {
        qWarning() << "Cannot open db!!!";
    }
    initProvider();
    initLabels();
    initDatasets();
    initFolders();
    initEntry();
    qDebug() << QSqlDatabase::database().tables();
}

TableModel *DatabaseHandler::providers() const
{
    return m_providers;
}

TableModel *DatabaseHandler::labels() const
{
    return m_labels;
}

TableModel *DatabaseHandler::datasets() const
{
    return m_datasets;
}

TableModel *DatabaseHandler::entries() const
{
    return m_entries;
}

void DatabaseHandler::setProviders(TableModel *providers)
{
    if (m_providers == providers)
        return;

    if (m_providers)
        delete m_providers;
    m_providers = providers;
    QObject::connect(m_providers, SIGNAL(beforeInsert(QSqlRecord &)),
                     m_providers, SLOT(updateCount(QSqlRecord &)));
    emit providersChanged(m_providers);
}

void DatabaseHandler::setLabels(TableModel *labels)
{
    if (m_labels == labels)
        return;

    if (m_labels)
        delete m_labels;
    m_labels = labels;
    QObject::connect(m_labels, SIGNAL(beforeInsert(QSqlRecord &)),
                     m_labels, SLOT(updateCount(QSqlRecord &)));
    emit labelsChanged(m_labels);
}

void DatabaseHandler::setDatasets(TableModel *datasets)
{
    if (m_datasets == datasets)
        return;

    if (m_datasets)
        delete m_datasets;
    m_datasets = datasets;
    QObject::connect(m_datasets, SIGNAL(beforeInsert(QSqlRecord &)),
                     m_datasets, SLOT(updateCount(QSqlRecord &)));
    emit datasetsChanged(m_datasets);
}

void DatabaseHandler::setEntries(TableModel *entries)
{
    if (m_entries == entries)
        return;

    if (m_entries)
        delete m_entries;
    m_entries = entries;
    QObject::connect(m_entries, SIGNAL(beforeInsert(QSqlRecord &)),
                     m_entries, SLOT(updateCount(QSqlRecord &)));
    emit entriesChanged(m_entries);
}

void DatabaseHandler::setFolders(TableModel *folders)
{
    if (m_folders == folders)
        return;

    if (m_folders)
        delete m_folders;
    m_folders = folders;
    QObject::connect(m_folders, SIGNAL(beforeInsert(QSqlRecord &)),
                     m_folders, SLOT(updateCount(QSqlRecord &)));
    emit foldersChanged(m_folders);
}

void DatabaseHandler::initProvider()
{
    auto providers = new TableModel();

    QSqlQuery query;
    if (query.exec("CREATE TABLE Providers (id integer PRIMARY KEY, name varchar(40) UNIQUE)")) {
        query.exec("INSERT INTO Providers VALUES (0, 'Folder')");
        query.exec("INSERT INTO Providers VALUES (1, 'Video')");
        query.exec("INSERT INTO Providers VALUES (2, 'CSV')");
    }

    providers->setTable("Providers");
    providers->setHeaderData(0, Qt::Horizontal, "id");
    providers->setHeaderData(1, Qt::Horizontal, "name");
    providers->generateRoles();
    providers->select();

    setProviders(providers);
    providers->updateCount();
}

void DatabaseHandler::initLabels()
{
    auto labels = new TableModel();
    QSqlQuery query;
    query.exec("CREATE TABLE Labels (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(40) UNIQUE)");

    labels->setTable("Labels");
    labels->setEditStrategy(QSqlTableModel::OnManualSubmit);
    labels->setHeaderData(0, Qt::Horizontal, "id");
    labels->setHeaderData(1, Qt::Horizontal, "name");
    labels->generateRoles();
    labels->select();

    setLabels(labels);
    labels->updateCount();
}

void DatabaseHandler::initFolders()
{
    auto folders = new TableModel();
    QSqlQuery query;
    query.exec("CREATE TABLE Folders (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(128))");
    folders->setTable("Folders");
    folders->setEditStrategy(QSqlTableModel::OnManualSubmit);
    folders->setHeaderData(0, Qt::Horizontal, "id");
    folders->setHeaderData(1, Qt::Horizontal, "name");
    folders->generateRoles();
    folders->select();

    setFolders(folders);
    folders->updateCount();
}

void DatabaseHandler::initDatasets()
{
    auto datasets = new TableModel();
    QSqlQuery query;
    query.exec("CREATE TABLE Datasets (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(40) UNIQUE, path varchar(256), providerId integer)");
    datasets->setTable("Datasets");
    datasets->setRelation(3, QSqlRelation("Providers", "id", "name"));
    datasets->setEditStrategy(QSqlTableModel::OnManualSubmit);
    datasets->setHeaderData(0, Qt::Horizontal, "id");
    datasets->setHeaderData(1, Qt::Horizontal, "name");
    datasets->setHeaderData(2, Qt::Horizontal, "path");
    datasets->setHeaderData(3, Qt::Horizontal, "providerId");
    datasets->generateRoles();
    datasets->select();

    setDatasets(datasets);
    datasets->updateCount();
}


void DatabaseHandler::initEntry()
{
    auto entries = new TableModel();
    QSqlQuery query;
    query.exec("CREATE TABLE Entries (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, filename varchar(128) UNIQUE, labelId integer, datasetId integer, folderId integer)");
    entries->setTable("Entries");
    entries->setRelation(2, QSqlRelation("Labels", "id", "name"));
    entries->setRelation(3, QSqlRelation("Datasets", "id", "name"));
    entries->setRelation(4, QSqlRelation("Folders", "id", "name"));
    entries->setEditStrategy(QSqlTableModel::OnManualSubmit);
    entries->setHeaderData(0, Qt::Horizontal, "id");
    entries->setHeaderData(1, Qt::Horizontal, "filename");
    entries->setHeaderData(2, Qt::Horizontal, "labelId");
    entries->setHeaderData(3, Qt::Horizontal, "datasetId");
    entries->setHeaderData(4, Qt::Horizontal, "folderId");

    setEntries(entries);
    entries->updateCount();
}

void DatabaseHandler::setLabelNames(const QStringList &labelNames)
{
    bool changed = false;
    for (const auto& l : labelNames)
    {
        if (l.isEmpty()) continue;
        QSqlQuery query;
        query.prepare("INSERT INTO Labels (name) VALUES (?)");
        query.addBindValue(l);
        if (query.exec())
            changed = true;
    }
    if (changed && m_labels->submitAll())
        emit labelsChanged(m_labels);
}

void DatabaseHandler::removeLabel(const int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Labels WHERE id = (?)");
    query.addBindValue(id);
    if (query.exec())
    {
        m_labels->submitAll();
        emit labelsChanged(m_labels);
    }
}

TableModel *DatabaseHandler::folders() const
{
    return m_folders;
}

