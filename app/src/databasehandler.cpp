#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QDebug>
#include <QThread>

#include "databasehandler.h"

DatabaseHandler::DatabaseHandler(QObject *parent) : QObject (parent)
{
    m_labels = nullptr;
    m_providers = nullptr;
    m_datasets = nullptr;
    m_entries = nullptr;
    m_folders = nullptr;
    m_folderStatus = nullptr;
    m_dbConnectionName = "ml_studio_" + QString::number((quint64)QThread::currentThread(), 16);
    qDebug() << m_dbConnectionName;
}

void DatabaseHandler::initDatabases(const QString &dbpath)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", m_dbConnectionName);
    db.setHostName("dnai");
    db.setDatabaseName(dbpath);
    db.setUserName("user");
    db.setPassword("dn41");
    if (!db.open()) {
        qWarning()  << "==App.DataBaseHandler== " << "Cannot open db!!!";
    }
    initFolderStatus();
    initProvider();
    initLabels();
    initDatasets();
    initFolders();
    initEntry();
    qDebug() << "==App.DataBaseHandler== " << QSqlDatabase::database(m_dbConnectionName).tables();
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

void DatabaseHandler::setFolderStatus(TableModel *folderStatus)
{
    if (m_folderStatus == folderStatus)
        return;

    delete m_providers;
    m_folderStatus = folderStatus;
    QObject::connect(m_folderStatus, SIGNAL(beforeInsert(QSqlRecord&)),
                     m_folderStatus, SLOT(updateCount(QSqlRecord&)));
    emit providersChanged(m_folderStatus);
    folderStatus->updateCount();
}

void DatabaseHandler::setProviders(TableModel *providers)
{
    if (m_providers == providers)
        return;

    delete m_providers;
    m_providers = providers;
    QObject::connect(m_providers, SIGNAL(beforeInsert(QSqlRecord&)),
                     m_providers, SLOT(updateCount(QSqlRecord&)));
    emit providersChanged(m_providers);
    providers->updateCount();
}

void DatabaseHandler::setLabels(TableModel *labels)
{
    if (m_labels == labels)
        return;

    delete m_labels;
    m_labels = labels;
    QObject::connect(m_labels, SIGNAL(beforeInsert(QSqlRecord&)),
                     m_labels, SLOT(updateCount(QSqlRecord&)));
    QObject::connect(m_labels, SIGNAL(beforeInsert(QSqlRecord&)),
                     this, SLOT(updateEntriesRelations(QSqlRecord&)));
    emit labelsChanged(m_labels);
    labels->updateCount();
}

void DatabaseHandler::setDatasets(TableModel *datasets)
{
    if (m_datasets == datasets)
        return;

    delete m_datasets;
    m_datasets = datasets;
    emit datasetsChanged(m_datasets);
    datasets->updateCount();
}

void DatabaseHandler::setEntries(TableModel *entries)
{
    if (m_entries == entries)
        return;

    delete m_entries;
    m_entries = entries;
    QObject::connect(m_entries, SIGNAL(beforeInsert(QSqlRecord&)),
                     m_entries, SLOT(updateCount(QSqlRecord&)));
    emit entriesChanged(m_entries);
    entries->updateCount();
}

void DatabaseHandler::setFolders(TableModel *folders)
{
    if (m_folders == folders)
        return;

    delete m_folders;
    m_folders = folders;
    QObject::connect(m_folders, SIGNAL(beforeInsert(QSqlRecord&)),
                     m_folders, SLOT(updateCount(QSqlRecord&)));
    emit foldersChanged(m_folders);
    folders->updateCount();
}

void DatabaseHandler::initProvider()
{
    qDebug() << "=======================================>" << QSqlDatabase::database(m_dbConnectionName);
    auto providers = new TableModel(nullptr, QSqlDatabase::database(m_dbConnectionName));

    QSqlQuery query(providers->database());
    auto isInit = query.exec("CREATE TABLE Providers (id integer PRIMARY KEY, name varchar(40) UNIQUE)");

    providers->setTable("Providers");
    providers->setHeaderData(0, Qt::Horizontal, "id");
    providers->setHeaderData(1, Qt::Horizontal, "name");
    providers->generateRoles();
    providers->select();

    if (isInit)
    {
        auto rec = m_factory.create(RecordType::Provider);
        rec->setValue(1, "Folder");
        providers->insertRecord(-1, *rec);
        delete rec;

        rec = m_factory.create(RecordType::Provider);
        rec->setValue(1, "MP4");
        providers->insertRecord(-1, *rec);
        delete rec;

        rec = m_factory.create(RecordType::Provider);
        rec->setValue(1, "CSV");
        providers->insertRecord(-1, *rec);
        delete rec;
    }

    setProviders(providers);
}

void DatabaseHandler::initFolderStatus()
{
    auto folderStatus = new TableModel(nullptr, QSqlDatabase::database(m_dbConnectionName));

    QSqlQuery query(folderStatus->database());
    auto isInit = query.exec("CREATE TABLE FolderStatus (id integer PRIMARY KEY, name varchar(40) UNIQUE)");

    folderStatus->setTable("FolderStatus");
    folderStatus->setHeaderData(0, Qt::Horizontal, "id");
    folderStatus->setHeaderData(1, Qt::Horizontal, "name");
    folderStatus->generateRoles();
    folderStatus->select();

    if (isInit)
    {
        auto rec = m_factory.create(RecordType::FolderStatus);
        rec->setValue(1, "NOT_LOADED");
        folderStatus->insertRecord(-1, *rec);
        delete rec;

        rec = m_factory.create(RecordType::FolderStatus);
        rec->setValue(1, "LOADED");
        folderStatus->insertRecord(-1, *rec);
        delete rec;

        rec = m_factory.create(RecordType::FolderStatus);
        rec->setValue(1, "SYNCHRONISED");
        folderStatus->insertRecord(-1, *rec);
        delete rec;
    }

    setProviders(folderStatus);
}

void DatabaseHandler::initLabels()
{
    auto labels = new TableModel(nullptr, QSqlDatabase::database(m_dbConnectionName));
    QSqlQuery query(labels->database());
    auto isInit = query.exec("CREATE TABLE Labels (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(40) UNIQUE)");

    labels->setTable("Labels");
    labels->setEditStrategy(QSqlTableModel::OnManualSubmit);
    labels->setHeaderData(0, Qt::Horizontal, "id");
    labels->setHeaderData(1, Qt::Horizontal, "name");
    labels->generateRoles();
    labels->select();

    if (isInit)
    {
        auto rec = m_factory.create(RecordType::Label);
        rec->setValue(1, "NOT_SET");
        labels->insertRecord(-1, *rec);
        delete rec;
    }
    labels->submitAll();

    setLabels(labels);
}

void DatabaseHandler::initFolders()
{
    auto folders = new TableModel(nullptr, QSqlDatabase::database(m_dbConnectionName));
    QSqlQuery query(folders->database());
    query.exec("CREATE TABLE Folders (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(128), datasetId integer, folderStatusId integer, labelId integer)");
    folders->setTable("Folders");
    folders->setRelation(2, QSqlRelation("Datasets", "id", "name"));
    folders->setRelation(3, QSqlRelation("FolderStatus", "id", "name"));
    folders->setRelation(4, QSqlRelation("Labels", "id", "name"));
    folders->setEditStrategy(QSqlTableModel::OnManualSubmit);
    folders->setHeaderData(0, Qt::Horizontal, "id");
    folders->setHeaderData(1, Qt::Horizontal, "name");
    folders->setHeaderData(2, Qt::Horizontal, "datasetId");
    folders->setHeaderData(3, Qt::Horizontal, "folderStatusId");
    folders->setHeaderData(4, Qt::Horizontal, "labelId");
    folders->generateRoles();
    folders->select();

    qDebug() << folders->relation(2).isValid()
             << folders->relation(3).isValid()
             << folders->relation(4).isValid();

    setFolders(folders);
}

void DatabaseHandler::initDatasets()
{
    auto datasets = new TableModel(nullptr, QSqlDatabase::database(m_dbConnectionName));
    QSqlQuery query(datasets->database());
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
}



void DatabaseHandler::initEntry()
{
    auto entries = new TableModel(nullptr, QSqlDatabase::database(m_dbConnectionName));
    QSqlQuery query(entries->database());
    query.exec("CREATE TABLE Entries (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, filename varchar(128), labelId integer, datasetId integer, folderId integer)");
    entries->setTable("Entries");

    entries->setRelation(3, QSqlRelation("Datasets", "id", "name"));
    entries->setRelation(4, QSqlRelation("Folders", "id", "name"));
    entries->setEditStrategy(QSqlTableModel::OnManualSubmit);
    entries->setHeaderData(0, Qt::Horizontal, "id");
    entries->setHeaderData(1, Qt::Horizontal, "filename");
    entries->setHeaderData(2, Qt::Horizontal, "labelId");
    entries->setHeaderData(3, Qt::Horizontal, "datasetId");
    entries->setHeaderData(4, Qt::Horizontal, "folderId");
    entries->generateRoles();
    entries->select();

    setEntries(entries);
}

TableModel *DatabaseHandler::createFolderEntries()
{
    auto entries = new TableModel(nullptr, QSqlDatabase::database(m_dbConnectionName));
    entries->setTable("Folders");

    entries->setRelation(2, QSqlRelation("Datasets", "id", "name"));
    entries->setRelation(3, QSqlRelation("FolderStatus", "id", "name"));
    entries->setRelation(4, QSqlRelation("Labels", "id", "name"));
    entries->setEditStrategy(QSqlTableModel::OnManualSubmit);
    entries->setHeaderData(0, Qt::Horizontal, "id");
    entries->setHeaderData(1, Qt::Horizontal, "name");
    entries->setHeaderData(2, Qt::Horizontal, "datasetId");
    entries->setHeaderData(3, Qt::Horizontal, "folderStatusId");
    entries->setHeaderData(4, Qt::Horizontal, "labelId");
    entries->generateRoles();
    entries->select();

    return entries;
}

TableModel *DatabaseHandler::folderStatus() const
{
    return m_folderStatus;
}

TableModel *DatabaseHandler::createFolderEntries(int id)
{
    auto entries = createFolderEntries();

    entries->setFilter("datasetId=" + QString::number(id));
    return entries;
}

void DatabaseHandler::updateEntriesRelations(QSqlRecord &rec)
{
    qDebug()  << "==App.DataBaseHandler== " << "updateEntriesRelations";
    Q_UNUSED(rec)
    qDebug()  << "==App.DataBaseHandler== " << "setup relations" << m_labels->rowCount() << m_entries->relation(2).isValid();
    if (m_labels->rowCount() > 0 && !m_entries->relation(2).isValid())
    {
        qDebug()  << "==App.DataBaseHandler== " << "setup relations";
        m_entries->setRelation(2, QSqlRelation("Labels", "id", "name"));
    }
}

void DatabaseHandler::setLabelNames(const QStringList &labelNames)
{
    bool changed = false;
    for (const auto& l : labelNames)
    {
        if (l.isEmpty()) continue;
        auto rec = m_factory.create(RecordType::Label);
        rec->setValue(1, l);
        if (m_labels->insertRecord(-1, *rec))
            changed = true;
        delete rec;
    }
    if (changed)
    {
        m_labels->submitAll();
        emit labelsChanged(m_labels);
    }
}

void DatabaseHandler::removeLabel(const int id)
{
    QSqlQuery query(m_labels->database());
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

const RecordFactory &DatabaseHandler::recordFactory() const
{
    return m_factory;
}

