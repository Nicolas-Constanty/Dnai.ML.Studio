#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlError>

#include "datasethandler.h"
#include "folderprovider.h"
#include "editor.h"
#include "parsefoldertask.h"

QVector<AProvider *> DatasetHandler::providers = {
    new FolderProvider()
};

QMap<QString, AProvider *> DatasetHandler::providerNames {
    {"Folder", DatasetHandler::providers[0]}
};


DatasetHandler::DatasetHandler(QObject *parent) : QObject (parent), m_currentDatasetIndex(0)
{
    m_dbh = Editor::instance().databaseHandler();
}

AProvider *DatasetHandler::createProvider(const QString &path)
{
    const auto type = checkFile(path);
	AProvider *provider = nullptr;
	switch (type)
	{
    case AProvider::DIR:
        return new FolderProvider();
    case AProvider::MP4:
        qWarning()  << "==App.DatasetHandler== " << "Mp4 provider is not ready!";
        return nullptr;
    case AProvider::CSV:
        qWarning()  << "==App.DatasetHandler== " << "CSV provider is not ready!";
        return nullptr;
    case AProvider::INVALID: return nullptr;
	}
    return provider;
}

void DatasetHandler::initDatabaseHandler()
{
    m_dbh = Editor::instance().databaseHandler();
}

void DatasetHandler::refreshFolders()
{
    auto dts = m_dbh->datasets();
    for (auto i = 0; i < dts->rowCount(); i++)
        m_folders.append(m_dbh->createFolderEntries(i+1));
}

void DatasetHandler::createDatasetFromPath(const QString &path)
{
    qDebug() << "Dataset start";
    auto p = QUrl(path).toLocalFile();
    auto datasetType = checkFile(path);
    qDebug() << "Dataset start 1";


    if (!createDatasetEntry(datasetType, p)) {
        qWarning()  << "==App.DatasetHandler== " << "Cannot create new dataset entry with path : " << path;
        return;
    }

    providers[datasetType - 1]->preload(p, m_dbh->datasets()->rowCount() + 1);
    m_folders.append(m_dbh->createFolderEntries(m_dbh->datasets()->rowCount() + 1));
    m_dbh->datasets()->submitAll();
    m_dbh->datasets()->updateCount();

}


int DatasetHandler::foldersCount() const
{
    return m_folders.count();
}

void DatasetHandler::appendFolder(QQmlListProperty<TableModel>* list, TableModel* p) {
    static_cast< DatasetHandler* >(list->data)->appendFolder(p);
}

void DatasetHandler::clearFolders(QQmlListProperty<TableModel>* list) {
    static_cast< DatasetHandler* >(list->data)->clearFolders();
}

TableModel* DatasetHandler::folder(QQmlListProperty<TableModel>* list, int i) {
    return static_cast< DatasetHandler* >(list->data)->folder(i);
}

int DatasetHandler::foldersCount(QQmlListProperty<TableModel>* list) {
    return static_cast< DatasetHandler* >(list->data)->foldersCount();
}

bool DatasetHandler::createDatasetEntry(AProvider::Type t, const QString & path)
{
    QSqlQuery q;

    q.prepare("INSERT into Datasets (id, name, path, providerId) values (?, ?, ?, ?)");
    q.bindValue(0, m_dbh->datasets()->rowCount() + 1);
    q.bindValue(1, QString("Dataset n° ") + QString::number(m_dbh->datasets()->rowCount() + 1));
    q.bindValue(2, path);
    q.bindValue(3, t);

    if (!q.exec()) {
        qDebug() << q.lastError();
        return false;
    }
    return true;
//    QSqlRecord rec;
//    rec.append(QSqlField("id", QVariant::Int));
//    rec.append(QSqlField("name", QVariant::String));
//    rec.append(QSqlField("path", QVariant::String));
//    rec.append(QSqlField("providerId", QVariant::Int));

//    rec.setValue(1, QString("Dataset n° ") + QString::number(m_dbh->datasets()->rowCount() + 1));
//    rec.setValue(2, path);
//    rec.setValue(3, t);

//    return m_dbh->datasets()->insertRecord(-1, rec);
}

bool DatasetHandler::validFile(const QString &path)
{
    return checkFile(path) != AProvider::INVALID;
}

void DatasetHandler::appendFolder(TableModel *model)
{
    m_folders.append(model);
    emit foldersChanged(folders());
}

TableModel *DatasetHandler::folder(int index) const
{
    if (index < 0 || index >= m_folders.length())
        return nullptr;
    return m_folders.at(index);
}

AProvider::Type DatasetHandler::checkFile(const QString &path)
{
    QFileInfo infos(QUrl(path).toLocalFile());
    if (!infos.exists())
    {
        qWarning()  << "==App.DatasetHandler== " << "File:" << path << "doesn't exist!";
        qWarning()  << "==App.DatasetHandler== " << "Abort dataset creation!";
        return AProvider::INVALID;
    }

    if (infos.isDir())
        return AProvider::DIR;
    if (infos.isFile() && infos.suffix() == "csv")
        return AProvider::CSV;
    if (infos.isFile() && infos.suffix() == "mp4")
        return AProvider::MP4;

    return AProvider::INVALID;
}
int DatasetHandler::currentDatasetIndex() const
{
    return m_currentDatasetIndex;
}

//Dataset *DatasetHandler::currentDataset() const
//{

//    if (m_datasetlist.count() < 1)
//        return nullptr;
//    return m_datasetlist[m_currentDatasetIndex];
//}

void DatasetHandler::setCurrentDatasetIndex(const int currentDatasetIndex)
{
    if (m_currentDatasetIndex == currentDatasetIndex)
        return;

    m_currentDatasetIndex = currentDatasetIndex;
    emit currentDatasetIndexChanged(m_currentDatasetIndex);
    //    emit currentDatasetChanged(currentDataset());
}


QQmlListProperty<TableModel> DatasetHandler::folders()
{
    return {
        this, this,
        &DatasetHandler::appendFolder,
        &DatasetHandler::foldersCount,
        &DatasetHandler::folder,
        &DatasetHandler::clearFolders
    };
}

//Dataset *DatasetHandler::dataset(const int index) const
//{
//    return nullptr;//m_datasets.at(index);
//}

void DatasetHandler::clearFolders() {
//    m_datasets.clear();
}


QString DatasetHandler::getLabel(const int index) const
{
    return m_dbh->labels()->record(index).value("name").toString();
}

void DatasetHandler::generateFolderEntries(int index)
{
    const auto folder = this->folder(index);
    const auto rec = m_dbh->datasets()->record(index);
    auto dirPath = rec.value(2).toString();
    auto id = rec.value(0).toInt();
    if (folder) {
        for (auto i = 0; i < folder->rowCount(); i++)
        {
            const auto r = folder->record(i);
            auto path = dirPath + "/" + r.value(1).toString();
            parseFolder(path, id, r.value(0).toInt());
        }
    }
}

void DatasetHandler::getFolderEntries(FolderInfos *l)
{
//     ->
}

void DatasetHandler::parseFolder(const QString &path, int datasetId, int folderId)
{
//    qDebug() << path;
    QStringList filters;
    filters << "*.jpg" << "*.png";
    Editor::instance().
            taskManager()->
            scheduleTask(new ProgressTask(new ParseFolderTask(
                                              datasetId, folderId, static_cast<FolderProvider *>(providers[0]),
                                              path, filters, QDir::Files),
                                          "Loading folder entries"));
}


