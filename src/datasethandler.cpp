#include <QFileInfo>
#include <QUrl>
#include <QDebug>

#include "datasethandler.h"
#include "dataset.h"



DatasetHandler::DatasetHandler(QObject *parent) : QObject (parent), m_currentDatasetIndex(0)
{

}

void DatasetHandler::createDatasetFromPath(const QString &path)
{
    if (!validFile(path))
        return;
    auto dataset = new Dataset(path);
    dataset->setName(QString("Dataset n° ") + QString::number(m_datasets.count() + 1));
    appendDataset(dataset);
}

bool DatasetHandler::validFile(const QString &path)
{
    QFileInfo infos(QUrl(path).toLocalFile());
    if (!infos.exists())
    {
        qWarning() << "File:" << path << "doesn't exist!";
        qWarning() << "Abort dataset creation!";
        return false;
    }

    return infos.isDir() || (infos.isFile() && (infos.suffix() == "csv" || infos.suffix() == "mp4"));
}

int DatasetHandler::currentDatasetIndex() const
{
    return m_currentDatasetIndex;
}

Dataset *DatasetHandler::currentDataset() const
{
    if (m_currentDatasetIndex >= m_datasets.count())
        return nullptr;
    return m_datasets[m_currentDatasetIndex];
}

void DatasetHandler::setCurrentDatasetIndex(const int currentDatasetIndex)
{
    if (m_currentDatasetIndex == currentDatasetIndex)
        return;

    m_currentDatasetIndex = currentDatasetIndex;
    emit currentDatasetIndexChanged(m_currentDatasetIndex);
    emit currentDatasetChanged(currentDataset());
}

QQmlListProperty<Dataset> DatasetHandler::datasets()
{
    return QQmlListProperty<Dataset>(this, this,
             &DatasetHandler::appendDataset,
             &DatasetHandler::datasetCount,
             &DatasetHandler::dataset,
             &DatasetHandler::clearDatasets);
}

void DatasetHandler::appendDataset(Dataset* p) {
    m_datasets.append(p);
    emit datasetsChanged(datasets());
    emit datasetCountChanged(m_datasets.count());
}

Dataset *DatasetHandler::dataset(const int index) const
{
    return m_datasets.at(index);
}

void DatasetHandler::clearDatasets() {
    m_datasets.clear();
}

int DatasetHandler::datasetCount() const
{
    return m_datasets.count();
}

void DatasetHandler::appendDataset(QQmlListProperty<Dataset>* list, Dataset* p) {
    reinterpret_cast< DatasetHandler* >(list->data)->appendDataset(p);
}

void DatasetHandler::clearDatasets(QQmlListProperty<Dataset>* list) {
    reinterpret_cast< DatasetHandler* >(list->data)->clearDatasets();
}

Dataset* DatasetHandler::dataset(QQmlListProperty<Dataset>* list, int i) {
    return reinterpret_cast< DatasetHandler* >(list->data)->dataset(i);
}

int DatasetHandler::datasetCount(QQmlListProperty<Dataset>* list) {
    return reinterpret_cast< DatasetHandler* >(list->data)->datasetCount();
}
