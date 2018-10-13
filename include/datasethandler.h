#ifndef DATASET_HANDLER_H
#define DATASET_HANDLER_H

#include <QObject>
#include <QQmlListProperty>
#include <QStringList>
#include "dataset.h"

class DatasetHandler : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<Dataset> datasets READ datasets NOTIFY datasetsChanged)
	Q_PROPERTY(int datasetCount READ datasetCount NOTIFY datasetCountChanged)
	Q_PROPERTY(int currentDatasetIndex READ currentDatasetIndex WRITE setCurrentDatasetIndex NOTIFY currentDatasetIndexChanged)
    Q_PROPERTY(Dataset *currentDataset READ currentDataset NOTIFY currentDatasetChanged)
    Q_PROPERTY(QStringList labelNames READ labelNames WRITE setLabelNames NOTIFY labelNamesChanged)

public:
	explicit DatasetHandler(QObject * parent = nullptr);

    QQmlListProperty<Dataset> datasets();
	int datasetCount() const;

	int currentDatasetIndex() const;
    Dataset * currentDataset() const;
	void setCurrentDatasetIndex(int currentDatasetIndex);

public:
	Q_INVOKABLE void createDatasetFromPath(const QString & path);
	Q_INVOKABLE static bool validFile(const QString & path);
    Q_INVOKABLE Dataset* dataset(int index) const;
    Q_INVOKABLE void appendDataset(Dataset* p);
	Q_INVOKABLE void clearDatasets();

    QStringList labelNames() const;
    void setLabelNames(QStringList labelNames);

    Q_INVOKABLE void appendLabel(const QString &label);
    Q_INVOKABLE void removeLabel(const QString &label);
    Q_INVOKABLE void setLabel(int index, const QString &label);
    Q_INVOKABLE QString getLabel(int index);
	
signals:
	void currentDatasetIndexChanged(int index);
    void currentDatasetChanged(Dataset *dataset);
    void datasetsChanged(QQmlListProperty<Dataset> datasets);
	void datasetCountChanged(int count);
    void labelNamesChanged(QStringList labelNames);

private:
    static void appendDataset(QQmlListProperty<Dataset>* list, Dataset * p);
    static void clearDatasets(QQmlListProperty<Dataset>* list);
    static Dataset *dataset(QQmlListProperty<Dataset>* list, int i);
    static int datasetCount(QQmlListProperty<Dataset>* list);

	int m_currentDatasetIndex;
    QVector<Dataset*> m_datasets;
    QVector<QString> m_labelNames;
};
#endif //DATASET_HANDLER_H
