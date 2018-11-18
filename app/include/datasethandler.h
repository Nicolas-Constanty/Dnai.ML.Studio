#ifndef DATASET_HANDLER_H
#define DATASET_HANDLER_H

#include <QObject>
#include <QQmlListProperty>
#include "dataset.h"
#include "datasetmodel.h"

class DatasetHandler : public QObject
{
	Q_OBJECT
    Q_PROPERTY(DatasetModel *datasets READ datasets NOTIFY datasetsChanged)
	Q_PROPERTY(int datasetCount READ datasetCount NOTIFY datasetCountChanged)
	Q_PROPERTY(int currentDatasetIndex READ currentDatasetIndex WRITE setCurrentDatasetIndex NOTIFY currentDatasetIndexChanged)
    Q_PROPERTY(Dataset *currentDataset READ currentDataset NOTIFY currentDatasetChanged)
    Q_PROPERTY(DatasetModel *labelNames READ labelNames NOTIFY labelNamesChanged)

public:
	explicit DatasetHandler(QObject * parent = nullptr);
	static AProvider* createProvider(const QString& path);

    DatasetModel *datasets();
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

	DatasetModel *labelNames();
    Q_INVOKABLE void setLabelNames(const QStringList &labelNames);

    Q_INVOKABLE void appendLabel(const QString &label);
    Q_INVOKABLE void removeLabel(int id);
    Q_INVOKABLE void setLabel(int index, const QString &label);
    Q_INVOKABLE QString getLabel(int index) const;

    void initDB();
signals:
	void currentDatasetIndexChanged(int index);
    void currentDatasetChanged(Dataset *dataset);
    void datasetsChanged(DatasetModel *datasets);
	void datasetCountChanged(int count);
    void labelNamesChanged(DatasetModel *labelNames);

private slots:
private:
    static Dataset::Type checkFile(const QString & path);

	int m_currentDatasetIndex;
    DatasetModel *m_datasets;
    DatasetModel *m_labelNames;
    bool createDatasetEntry(Dataset::Type t, const QString &path);

    // INIT DB FUNCTIONS
    void initProvider();
    void initLabels();
    void initEntry();
    void initDatasets();

};
#endif //DATASET_HANDLER_H
