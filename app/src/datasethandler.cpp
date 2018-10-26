#include <QFileInfo>
#include <QUrl>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlField>

#include "datasethandler.h"
#include "dataset.h"
#include "folderprovider.h"


DatasetHandler::DatasetHandler(QObject *parent) : QObject (parent), m_currentDatasetIndex(0)
{
    m_labelNames = nullptr;
    m_datasets = nullptr;
}

void DatasetHandler::initDB() {

    QSqlQuery query;
    query.exec("CREATE TABLE Providers (id int PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(40) UNIQUE)");
    initLabels();
    query.exec("CREATE TABLE Entry (id int PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, filename varchar(128) UNIQUE, labelId int, datasetId int)");
    initDatasets();
}


void DatasetHandler::initLabels()
{
    auto n = new DatasetModel(nullptr, QSqlDatabase::database());
    if (m_labelNames)
    {
        auto t = m_labelNames;
        m_labelNames = n;
        delete t;
    }
    else
        m_labelNames = n;

    QSqlQuery query;
    query.exec("CREATE TABLE Labels (id int PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(40) UNIQUE)");
    m_labelNames->setTable("Labels");
    m_labelNames->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_labelNames->setHeaderData(0, Qt::Horizontal, "id");
    m_labelNames->setHeaderData(1, Qt::Horizontal, "name");
    m_labelNames->generateRoles();
    m_labelNames->select();
    emit labelNamesChanged(m_labelNames);
}

void DatasetHandler::initDatasets()
{
    auto n = new DatasetModel(nullptr, QSqlDatabase::database());
    if (m_datasets)
    {
        auto t = m_datasets;
        m_datasets = n;
        delete t;
    }
    else
        m_datasets = n;
    QSqlQuery query;
    query.exec("CREATE TABLE Datasets (id int PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, name varchar(40) UNIQUE, path varchar(256), providerId int)");

    m_datasets->setTable("Datasets");
    m_datasets->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_datasets->setHeaderData(0, Qt::Horizontal, "id");
    m_datasets->setHeaderData(1, Qt::Horizontal, "name");
    m_datasets->setHeaderData(2, Qt::Horizontal, "path");
    m_datasets->setHeaderData(3, Qt::Horizontal, "providerId");
    m_datasets->generateRoles();
    m_datasets->select();
    emit datasetsChanged(m_datasets);
}

AProvider *DatasetHandler::createProvider(const QString &path)
{
    const auto type = checkFile(path);
	AProvider *provider = nullptr;
	switch (type)
	{
	case Dataset::DIR:
        return new FolderProvider(path);
    case Dataset::MP4:
        qWarning() << "Mp4 provider is not ready!";
        return nullptr;
    case Dataset::CSV:
        qWarning() << "CSV provider is not ready!";
        return nullptr;
    case Dataset::INVALID: return nullptr;
	}
	return provider;
}

void DatasetHandler::createDatasetFromPath(const QString &path)
{
//	const auto provider = createProvider(path);
//	if (!provider) return;
//    auto dataset = new Dataset(*provider);
//	const auto datasetName = QString("Dataset n° ") + QString::number(m_datasets.count() + 1);
//    dataset->setName(datasetName);
//    appendDataset(dataset);
}

bool DatasetHandler::validFile(const QString &path)
{
    return checkFile(path) != Dataset::INVALID;
}

Dataset::Type DatasetHandler::checkFile(const QString &path)
{
    QFileInfo infos(QUrl(path).toLocalFile());
    if (!infos.exists())
    {
        qWarning() << "File:" << path << "doesn't exist!";
        qWarning() << "Abort dataset creation!";
        return Dataset::INVALID;
    }

    if (infos.isDir())
        return Dataset::DIR;
    if (infos.isFile() && infos.suffix() == "csv")
        return Dataset::CSV;
    if (infos.isFile() && infos.suffix() == "mp4")
        return Dataset::MP4;

    return Dataset::INVALID;
}
int DatasetHandler::currentDatasetIndex() const
{
    return m_currentDatasetIndex;
}

Dataset *DatasetHandler::currentDataset() const
{
    return nullptr;
//    if (m_currentDatasetIndex >= m_datasets.count())
//        return nullptr;
//    return m_datasets[m_currentDatasetIndex];
}

void DatasetHandler::setCurrentDatasetIndex(const int currentDatasetIndex)
{
    if (m_currentDatasetIndex == currentDatasetIndex)
        return;

    m_currentDatasetIndex = currentDatasetIndex;
    emit currentDatasetIndexChanged(m_currentDatasetIndex);
    emit currentDatasetChanged(currentDataset());
}

DatasetModel *DatasetHandler::datasets()
{
    return m_datasets;
}

//QQmlListProperty<Dataset> DatasetHandler::datasets()
//{
//    return QQmlListProperty<Dataset>(this, this,
//             &DatasetHandler::appendDataset,
//             &DatasetHandler::datasetCount,
//             &DatasetHandler::dataset,
//             &DatasetHandler::clearDatasets);
//}

void DatasetHandler::appendDataset(Dataset* p) {
    //m_datasets.append(p);
    emit datasetsChanged(datasets());
    //emit datasetCountChanged(m_datasets.count());
}

Dataset *DatasetHandler::dataset(const int index) const
{
    return nullptr;//m_datasets.at(index);
}

void DatasetHandler::clearDatasets() {
//    m_datasets.clear();
}

int DatasetHandler::datasetCount() const
{
    return -1;//m_datasets.count();
}

//void DatasetHandler::appendDataset(QQmlListProperty<Dataset>* list, Dataset* p) {
//    reinterpret_cast< DatasetHandler* >(list->data)->appendDataset(p);
//}

//void DatasetHandler::clearDatasets(QQmlListProperty<Dataset>* list) {
//    reinterpret_cast< DatasetHandler* >(list->data)->clearDatasets();
//}

//Dataset* DatasetHandler::dataset(QQmlListProperty<Dataset>* list, int i) {
//    return reinterpret_cast< DatasetHandler* >(list->data)->dataset(i);
//}

//int DatasetHandler::datasetCount(QQmlListProperty<Dataset>* list) {
//    return reinterpret_cast< DatasetHandler* >(list->data)->datasetCount();
//}

DatasetModel *DatasetHandler::labelNames()
{
    return m_labelNames;
}

void DatasetHandler::appendLabel(const QString& label)
{
	QSqlRecord rec;
	rec.setValue("NAME", label);
    if (m_labelNames->insertRecord(-1, rec) && m_labelNames->submitAll())
    {
        emit labelNamesChanged(m_labelNames);
    }
}

void DatasetHandler::removeLabel(const int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Labels WHERE id = (?)");
    query.addBindValue(id);
    if (query.exec())
    {
        m_labelNames->select();
        emit labelNamesChanged(m_labelNames);
    }
}

void DatasetHandler::setLabel(const int index, const QString& label)
{
	QSqlRecord rec;
	rec.setValue("NAME", label);
    if (m_labelNames->setRecord(index, rec))
        emit labelNamesChanged(m_labelNames);
}

QString DatasetHandler::getLabel(const int index) const
{
    return m_labelNames->record(index).value("name").toString();
}


void DatasetHandler::setLabelNames(const QStringList &labelNames)
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
    if (changed && m_labelNames->submitAll())
        emit labelNamesChanged(m_labelNames);
}


