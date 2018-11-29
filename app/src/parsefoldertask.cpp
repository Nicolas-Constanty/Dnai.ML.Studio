#include <QThread>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "parsefoldertask.h"
#include "editor.h"

ParseFolderTask::ParseFolderTask(int index, int datasetId, int folderId, QObject *receiver,
                                 const QString &path,
                                 const QStringList &filters,
                                 QDir::Filters flags) :
    QRunnableProgress(),
    m_index(index),
    m_datasetId(datasetId),
    m_folderId(folderId),
    m_path(path),
    m_filters(filters),
    m_flags(flags)
{
    QObject::connect(this, SIGNAL(sendResults(int, int)),
                     receiver, SLOT(updateFolder(int, int)));
}

void ParseFolderTask::run()
{
    auto name = "ml_studio_" + QString::number((quint64)QThread::currentThread(), 16);
    QSqlDatabase db;
    if(QSqlDatabase::contains(name))
        db = QSqlDatabase::database(name);
    else {
        db = QSqlDatabase::addDatabase( "QSQLITE", name);
        db.setDatabaseName(Editor::instance().project()->dbPath());
        if (!db.open()) {
            qWarning()  << "==App.DataBaseHandler== " << "Cannot open db!!!";
        }
    }

    QDir dir(m_path);
    FolderInfos folderInfos;
    auto files = dir.entryList(m_filters, m_flags,QDir::SortFlag::Name);

    folderInfos.files = files;
    folderInfos.datasetId = m_datasetId;
    folderInfos.folderId = m_folderId;
    bulkInsert(&folderInfos, db);
    QThread::usleep(0);
}

bool ParseFolderTask::bulkInsertRequest(EntryRecord &record, const QSqlDatabase &db)
{
    QSqlQuery q(db);

    q.prepare("INSERT into Entries values(?, ?, ?, ?, ?)");
    q.addBindValue(record.ids);
    q.addBindValue(record.filenames);
    q.addBindValue(record.labelIds);
    q.addBindValue(record.datasetIds);
    q.addBindValue(record.folderIds);

    if (!q.execBatch())
    {
        qDebug() << q.lastError();
        return false;
    }

    record.clear();
    return true;
}


void ParseFolderTask::bulkInsert(FolderInfos *infos, const QSqlDatabase& db) {
    QSqlQuery q(db);

    int startId = 0;
    if (q.exec("SELECT COUNT(*) FROM Entries"))
    {
        q.first();
        startId = q.value(0).toInt() + 1;
    } else {
        qDebug() << "==App.ParseFolderTask==" << q.lastError();
        return;
    }

    EntryRecord record;
//    auto maxBulk = 100;
    auto count = infos->files.count();
    auto index = 0.0;
    for (auto file : infos->files)
    {
        setProgress(index++ / count);
        setProgressText(file);
        QThread::usleep(0);
        record.appendRecord(startId++, file, 0, m_datasetId, m_folderId);
//        if (record.count() == maxBulk)
//        {
//            bulkInsertRequest(record, db);
//        }
    }
    setProgressText("Saving database entries ...");
    QThread::usleep(0);
    if (record.count() > 0)
        bulkInsertRequest(record, db);
    emit sendResults(m_folderId, m_index);
    QThread::usleep(0);
}
