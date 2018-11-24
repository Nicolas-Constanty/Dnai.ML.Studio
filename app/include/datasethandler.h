#ifndef DATASET_HANDLER_H
#define DATASET_HANDLER_H

#include <QObject>
#include <QQmlListProperty>
#include "dataset.h"
#include "datasetmodel.h"
#include "databasehandler.h"

class DatasetHandler : public QObject
{
	Q_OBJECT
    Q_PROPERTY(QQmlListProperty<TableModel> folders READ folders NOTIFY foldersChanged)
	Q_PROPERTY(int currentDatasetIndex READ currentDatasetIndex WRITE setCurrentDatasetIndex NOTIFY currentDatasetIndexChanged)
//    Q_PROPERTY(Dataset *currentDataset READ currentDataset NOTIFY currentDatasetChanged)

public:
    explicit DatasetHandler(QObject * parent = nullptr);
	static AProvider* createProvider(const QString& path);

    void initDatabaseHandler();

	int currentDatasetIndex() const;
//    Dataset * currentDataset() const;
	void setCurrentDatasetIndex(int currentDatasetIndex);

    QQmlListProperty<TableModel> folders();

public:
	Q_INVOKABLE void createDatasetFromPath(const QString & path);
    Q_INVOKABLE static bool validFile(const QString & path);


    Q_INVOKABLE void appendFolder(TableModel *model);
    Q_INVOKABLE TableModel *folder(int index) const;
    Q_INVOKABLE void clearFolders();
    Q_INVOKABLE int foldersCount() const;

    Q_INVOKABLE QString getLabel(int index) const;


signals:
	void currentDatasetIndexChanged(int index);
    void foldersChanged(const QQmlListProperty<TableModel> &folders);
//    void currentDatasetChanged(Dataset *dataset);

private slots:
private:
    static Dataset::Type checkFile(const QString & path);
    static void appendFolder(QQmlListProperty<TableModel>* list, TableModel * p);
    static void clearFolders(QQmlListProperty<TableModel>* list);
    static TableModel *folder(QQmlListProperty<TableModel>* list, int i);
    static int foldersCount(QQmlListProperty<TableModel>* list);

    int m_currentDatasetIndex;
    DatabaseHandler *m_dbh;
//    QVector<Dataset *> m_datasetlist;
    bool createDatasetEntry(Dataset::Type t, const QString &path);

    void refreshDatasetList();

    static QVector<AProvider *> providers;
    QVector<TableModel *> m_folders;
};
#endif //DATASET_HANDLER_H
