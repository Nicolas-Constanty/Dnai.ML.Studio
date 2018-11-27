#ifndef PARSEFOLDERTASK_H
#define PARSEFOLDERTASK_H

#include <QDir>
#include <QDirIterator>
#include <QDateTime>

#include "progresstask.h"


struct FolderInfos {
    QFileInfoList list;
    int datasetId;
    int folderId;
};

struct QDirSortItem
{
    mutable QString filename_cache;
    mutable QString suffix_cache;
    QFileInfo item;
};

class QDirSortItemComparator
{
    int qt_cmp_si_sort_flags;
public:
    QDirSortItemComparator(int flags);
    bool operator()(const QDirSortItem &, const QDirSortItem &) const;
};


class ParseFolderTask : public QRunnableProgress {
    Q_OBJECT

signals:
    void sendResults(FolderInfos *infos);
public:
    ParseFolderTask(int datasetId, int folderId, QObject *receiver,
                    const QString &path,
                    const QStringList &filters,
                    QDir::Filters flags = QDir::NoFilter);

    void run() override;

private:
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
};

#endif // PARSEFOLDERTASK_H
