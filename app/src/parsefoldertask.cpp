#include <QThread>
#include <QDebug>
#include "parsefoldertask.h"

QDirSortItemComparator::QDirSortItemComparator(int flags) : qt_cmp_si_sort_flags(flags) {}

bool QDirSortItemComparator::operator()(const QDirSortItem &n1, const QDirSortItem &n2) const
{
    const QDirSortItem* f1 = &n1;
    const QDirSortItem* f2 = &n2;
    if ((qt_cmp_si_sort_flags & QDir::DirsFirst) && (f1->item.isDir() != f2->item.isDir()))
        return f1->item.isDir();
    if ((qt_cmp_si_sort_flags & QDir::DirsLast) && (f1->item.isDir() != f2->item.isDir()))
        return !f1->item.isDir();
    qint64 r = 0;
    int sortBy = (qt_cmp_si_sort_flags & QDir::SortByMask)
            | (qt_cmp_si_sort_flags & QDir::Type);
    switch (sortBy) {
    case QDir::Time: {
        QDateTime firstModified = f1->item.lastModified();
        QDateTime secondModified = f2->item.lastModified();
        // QDateTime by default will do all sorts of conversions on these to
        // find timezones, which is incredibly expensive. As we aren't
        // presenting these to the user, we don't care (at all) about the
        // local timezone, so force them to UTC to avoid that conversion.
        firstModified.setTimeSpec(Qt::UTC);
        secondModified.setTimeSpec(Qt::UTC);
        r = firstModified.msecsTo(secondModified);
        break;
    }
    case QDir::Size:
        r = f2->item.size() - f1->item.size();
        break;
    case QDir::Type:
    {
        bool ic = qt_cmp_si_sort_flags & QDir::IgnoreCase;
        if (f1->suffix_cache.isNull())
            f1->suffix_cache = ic ? f1->item.suffix().toLower()
                                  : f1->item.suffix();
        if (f2->suffix_cache.isNull())
            f2->suffix_cache = ic ? f2->item.suffix().toLower()
                                  : f2->item.suffix();
        r = qt_cmp_si_sort_flags & QDir::LocaleAware
                ? f1->suffix_cache.localeAwareCompare(f2->suffix_cache)
                : f1->suffix_cache.compare(f2->suffix_cache);
    }
        break;
    default:
        ;
    }
    if (r == 0 && sortBy != QDir::Unsorted) {
        // Still not sorted - sort by name
        bool ic = qt_cmp_si_sort_flags & QDir::IgnoreCase;
        if (f1->filename_cache.isNull())
            f1->filename_cache = ic ? f1->item.fileName().toLower()
                                    : f1->item.fileName();
        if (f2->filename_cache.isNull())
            f2->filename_cache = ic ? f2->item.fileName().toLower()
                                    : f2->item.fileName();
        r = qt_cmp_si_sort_flags & QDir::LocaleAware
                ? f1->filename_cache.localeAwareCompare(f2->filename_cache)
                : f1->filename_cache.compare(f2->filename_cache);
    }
    if (qt_cmp_si_sort_flags & QDir::Reversed)
        return r > 0;
    return r < 0;
}

ParseFolderTask::ParseFolderTask(int datasetId, int folderId, QObject *receiver,
                                 const QString &path,
                                 const QStringList &filters,
                                 QDir::Filters flags) :
    QRunnableProgress(),
    m_datasetId(datasetId),
    m_folderId(folderId),
    m_path(path),
    m_filters(filters),
    m_flags(flags)
{
    QObject::connect(this, SIGNAL(sendResults(FolderInfos *)),
                     receiver, SLOT(bulkInsert(FolderInfos *)));
}

void ParseFolderTask::run()
{
    QDirIterator it(m_path, m_filters, m_flags);
    QFileInfoList l;
    setIndeterminate(true);
    QThread::usleep(0);
    while (it.hasNext()) {
        it.next();
        l.append(it.fileInfo());
        setProgressText(it.fileName());
        QThread::usleep(0);
    }
    auto ret = new FolderInfos();
    ret->datasetId = m_datasetId;
    ret->folderId = m_folderId;
    sortFileList(QDir::SortFlag::Name, l, nullptr, &ret->list);
    emit sendResults(ret);
    QThread::usleep(0);
}

void ParseFolderTask::sortFileList(QDir::SortFlags sort, QFileInfoList &l, QStringList *names, QFileInfoList *infos)
{
    // names and infos are always empty lists or 0 here
    int n = l.size();
    if (n > 0) {
        if (n == 1 || (sort & QDir::SortByMask) == QDir::Unsorted) {
            if (infos)
                *infos = l;
            if (names) {
                for (int i = 0; i < n; ++i)
                    names->append(l.at(i).fileName());
            }
        } else {
            setProgressText("Sorting files...");
            QScopedArrayPointer<QDirSortItem> si(new QDirSortItem[n]);
            for (int i = 0; i < n; ++i)
                si[i].item = l.at(i);
            std::sort(si.data(), si.data() + n, QDirSortItemComparator(sort));
            // put them back in the list(s)
            if (infos) {
                for (int i = 0; i < n; ++i)
                    infos->append(si[i].item);
            }
            if (names) {
                for (int i = 0; i < n; ++i)
                    names->append(si[i].item.fileName());
            }
            setProgressText("Files sorted.");
        }
    }
}
