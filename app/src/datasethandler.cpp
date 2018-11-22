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
#include "editor.h"

AProvider *DatasetHandler::providers[] = {
    new FolderProvider()
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
	case Dataset::DIR:
        return new FolderProvider();
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

void DatasetHandler::initDatabaseHandler()
{
    m_dbh = Editor::instance().databaseHandler();
}

void DatasetHandler::refreshDatasetList()
{
//    for (auto i = 0; i < m_dbh->datasets()->rowCount(); i++)
//    {
//        qDebug() << m_dbh->datasets()->record(i);
//        qDebug() << m_dbh->datasets()->data(m_dbh->datasets()->index(i, 0), Qt::UserRole + 1);
//        qDebug() << m_dbh->datasets()->data(m_dbh->datasets()->index(i, 0), Qt::UserRole + 2);
//        qDebug() << m_dbh->datasets()->data(m_dbh->datasets()->index(i, 0), Qt::UserRole + 3);
//        qDebug() << m_dbh->datasets()->data(m_dbh->datasets()->index(i, 0), Qt::UserRole + 4);
//    }
}

void DatasetHandler::createDatasetFromPath(const QString &path)
{
    auto p = QUrl(path).toLocalFile();
    auto datasetType = checkFile(path);
    if (!createDatasetEntry(datasetType, p)) {
        qWarning() << "Cannot create new dataset entry with path : " << path;
        return;
    }

    providers[datasetType]->generateModel(p, m_dbh->datasets()->rowCount());

    m_dbh->datasets()->submitAll();
    m_dbh->entries()->submitAll();
}

bool DatasetHandler::createDatasetEntry(Dataset::Type t, const QString & path)
{
    QSqlRecord rec;
    rec.append(QSqlField("id", QVariant::Int));
    rec.append(QSqlField("name", QVariant::String));
    rec.append(QSqlField("path", QVariant::String));
    rec.append(QSqlField("providerId", QVariant::Int));

    rec.setValue(1, QString("Dataset n° ") + QString::number(m_dbh->datasets()->rowCount() + 1));
    rec.setValue(2, path);
    rec.setValue(3, t);

    return m_dbh->datasets()->insertRecord(-1, rec);
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

    if (m_datasetlist.count() < 1)
        return nullptr;
    return m_datasetlist[m_currentDatasetIndex];
}

void DatasetHandler::setCurrentDatasetIndex(const int currentDatasetIndex)
{
    if (m_currentDatasetIndex == currentDatasetIndex)
        return;

    m_currentDatasetIndex = currentDatasetIndex;
    emit currentDatasetIndexChanged(m_currentDatasetIndex);
    emit currentDatasetChanged(currentDataset());
}

Dataset *DatasetHandler::dataset(const int index) const
{
    return nullptr;//m_datasets.at(index);
}

void DatasetHandler::clearDatasets() {
//    m_datasets.clear();
}


QString DatasetHandler::getLabel(const int index) const
{
    return m_dbh->labels()->record(index).value("name").toString();
}



