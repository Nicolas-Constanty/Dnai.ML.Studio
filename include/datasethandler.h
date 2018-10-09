#ifndef DATASET_HANDLER_H
#define DATASET_HANDLER_H

#include <QObject>
#include <QQmlListProperty>
#include "dataset.h"

namespace dnai {
namespace mlstudio {

class DatasetHandler : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<Dataset> datasets READ datasets NOTIFY datasetsChanged)
	Q_PROPERTY(int datasetCount READ datasetCount NOTIFY datasetCountChanged)
	Q_PROPERTY(int currentDatasetIndex READ currentDatasetIndex WRITE setCurrentDatasetIndex NOTIFY currentDatasetIndexChanged)
	Q_PROPERTY(Dataset *currentDataset READ currentDataset NOTIFY currentDatasetChanged)

public:
	explicit DatasetHandler(QObject * parent = nullptr);

public slots:
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
	
signals:
	void currentDatasetIndexChanged(int index);
	void currentDatasetChanged(Dataset *dataset);
	void datasetsChanged(QQmlListProperty<Dataset> datasets);
	void datasetCountChanged(int count);

private:
	static void appendDataset(QQmlListProperty<Dataset>* list, Dataset * p);
	static void clearDatasets(QQmlListProperty<Dataset>* list);
	static Dataset *dataset(QQmlListProperty<Dataset>* list, int i);
	static int datasetCount(QQmlListProperty<Dataset>* list);

	int m_currentDatasetIndex;
	QVector<Dataset*> m_datasets;
};

}
}
#endif //DATASET_HANDLER_H
