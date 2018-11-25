#include <QDirIterator>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "folderprovider.h"
#include "editor.h"

void FolderProvider::initModel(const QString &name)
{
	m_data.setTable(name);
	m_data.setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_data.setHeaderData(0, Qt::Horizontal, "Filename");
	m_data.setHeaderData(1, Qt::Horizontal, "Label");
	m_data.setHeaderData(2, Qt::Horizontal, "xMin");
	m_data.setHeaderData(3, Qt::Horizontal, "xMax");
	m_data.setHeaderData(4, Qt::Horizontal, "yMin");
	m_data.setHeaderData(5, Qt::Horizontal, "yMax");
	m_data.select();
	m_init = true;
}

bool FolderProvider::bulkInsertRequest(EntryRecord &record, TableModel *db)
{
    QSqlQuery q;
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

    db->submitAll();
    db->select();

    record.clear();
    return true;
}

void FolderProvider::preload(const QString &path, int datasetId)
{
    QFileInfo infos(path);
    if (!infos.isDir())
    {
        qWarning()  << "==App.FolderProvider== " << "Warning : this isn't a directory path.";
        return;
    }

    QDir dir(path);
    auto dirs = dir.entryInfoList(QDir::Dirs, QDir::SortFlag::Name);
    qDebug() << dirs;
    QDir last;
    auto db = Editor::instance().databaseHandler();
    auto folders = db->folders();
    int startId = folders->rowCount() + 1;
    QVariantList ids;
    QVariantList nameIds;
    QVariantList datasetIds;
    QVariantList folderStatusIds;
    QVariantList labelsIds;
    for (auto it = dirs.constBegin(); it != dirs.constEnd(); it++) {
        if (it->fileName() == "." || it->fileName() == "..")
            continue;
        QDirIterator files(it->filePath(), QDir::Files);
        do {
            files.next();
            const auto suffix = files.fileInfo().suffix().toLower();
            if ((suffix == "jpg" || suffix == "png" || files.fileInfo().absoluteDir().entryList(QDir::Files).count() > 1048) && last != files.fileInfo().absoluteDir())
            {
                last = files.fileInfo().absoluteDir();
                ids << startId++;
                nameIds << last.dirName();
                datasetIds << datasetId;
                folderStatusIds << 1;
                labelsIds << 1;
                break;
            }
        } while(files.hasNext());
    };
    QSqlQuery q;
    q.prepare("INSERT into Folders values(?, ?, ?, ?, ?)");
    q.addBindValue(ids);
    q.addBindValue(nameIds);
    q.addBindValue(datasetIds);
    q.addBindValue(folderStatusIds);
    q.addBindValue(labelsIds);
    if (!q.execBatch())
        qDebug() << q.lastError();
    folders->submitAll();
    folders->select();
}

void FolderProvider::generateModel(const QString &path, int datasetId)
{
    QFileInfo infos(path);
	if (!infos.isDir())
    {
        qWarning()  << "==App.FolderProvider== " << "Warning : this isn't a directory path.";
		return;
    }

    QDirIterator dirs(path, QDir::Dirs);
    QDir last;
    auto db = Editor::instance().databaseHandler();
    auto folders = db->folders();
    auto entries = db->entries();
    int maxBulk = 100;
    int startId = db->entries()->rowCount() + 1;
    do {
        dirs.next();
        if (dirs.fileName() == "." || dirs.fileName() == "..")
            continue;
        QDirIterator files(dirs.filePath(), QDir::Files);
        QVector<QSqlRecord*> recs;
        EntryRecord record;
        do {
            files.next();
            const auto suffix = files.fileInfo().suffix().toLower();
            if (suffix == "jpg" || suffix == "png")
            {
                if (last != files.fileInfo().absoluteDir())
                {
                    last = files.fileInfo().absoluteDir();
                    auto rec = db->recordFactory().create(RecordType::Folder);
                    rec->setValue("name", last.dirName());
                    folders->insertRecord(-1, *rec);
                    folders->submitAll();
                    delete rec;
                }
                record.appendRecord(startId++, files.fileInfo().fileName(), 0, datasetId, folders->rowCount());
                if (record.count() == maxBulk)
                {
                    FolderProvider::bulkInsertRequest(record, entries);
                    qDebug() << record.count();
                }
            }
        } while(files.hasNext());
        if (record.count() > 0)
            FolderProvider::bulkInsertRequest(record, entries);
    } while(dirs.hasNext());
}


