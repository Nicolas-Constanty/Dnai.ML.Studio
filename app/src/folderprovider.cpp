#include <QDirIterator>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "folderprovider.h"
#include "editor.h"
#include "parsefoldertask.h"

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

void FolderProvider::preload(const QString &path, int datasetId)
{
    QFileInfo infos(path);
    if (!infos.isDir())
    {
        qWarning()  << "==App.FolderProvider== " << "Warning : this isn't a directory path.";
        return;
    }

    QDir dir(path);
    QStringList filters;
    filters << "*.jpg" << "*.png";
    auto dirs = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::SortFlag::Name);
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
        QDirIterator files(it->filePath(), filters, QDir::Files);
        do {
            files.next();
            if ((files.fileInfo().absoluteDir().entryList(QDir::Files).count() > 1024) && last != files.fileInfo().absoluteDir())
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
    QSqlQuery q(folders->database());
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

