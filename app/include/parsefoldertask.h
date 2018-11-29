#ifndef PARSEFOLDERTASK_H
#define PARSEFOLDERTASK_H

#include <QDir>
#include <QDirIterator>
#include <QDateTime>
#include <QVariantList>
#include <QDebug>

#include "progresstask.h"
#include "datasetmodel.h"


struct FolderInfos {
    QStringList files;
    int datasetId;
    int folderId;
};

class ParseFolderTask : public QRunnableProgress {
    Q_OBJECT
signals:
    void sendResults(int datasetId, int index);
public:
    class EntryRecord {
    public:
        QVariantList ids;
        QVariantList filenames;
        QVariantList labelIds;
        QVariantList datasetIds;
        QVariantList folderIds;

        void clear()
        {
            ids.clear();
            filenames.clear();
            labelIds.clear();
            datasetIds.clear();
            folderIds.clear();
            m_count = 0;
        }

        void appendRecord(int id, const QString &filename, int labelId, int datasetId, int folderId)
        {
            ids << id;
            filenames << filename;
            labelIds << labelId;
            datasetIds << datasetId;
            folderIds << folderId;
            m_count++;
        }

        int count()
        {
            return m_count;
        }

        friend QDebug operator<<(QDebug os, const EntryRecord& e)
        {
            os << e.ids << "\n" << e.filenames << "\n" << e.labelIds << "\n" << e.datasetIds << "\n" << e.folderIds;
            return os;
        }
    private:
        int m_count = 0;
    };

    ParseFolderTask(int index, int datasetId, int folderId, QObject *receiver,
                    const QString &path,
                    const QStringList &filters,
                    QDir::Filters flags = QDir::NoFilter);

    void run() override;

private:
    int m_index;
    int m_datasetId;
    int m_folderId;
    QString m_path;
    QStringList m_filters;
    QDir::Filters m_flags;
    QObject *m_receiver;

    void sortFileList(QDir::SortFlags sort,
                      QFileInfoList &l,
                      QStringList *names,
                      QFileInfoList *infos);
    void bulkInsert(FolderInfos *infos, const QSqlDatabase& db);
    bool bulkInsertRequest(EntryRecord &record, const QSqlDatabase &db);
};

#endif // PARSEFOLDERTASK_H
