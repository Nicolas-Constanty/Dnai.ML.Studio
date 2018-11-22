#include <QDirIterator>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQuery>

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

void FolderProvider::generateModel(const QString &path, int datasetId)
{
    QFileInfo infos(path);
	if (!infos.isDir())
    {
        qWarning() << "Warning : this isn't a directory path.";
		return;
    }

    QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
    QSqlQuery query;
    QDir last;
    QDir init(path);
    auto db = Editor::instance().databaseHandler();
    auto folders = db->folders();
	while (it.hasNext())
	{
        auto n = it.next();
        auto info = it.fileInfo();
        if (info.absoluteDir() == path)
            continue;
        if (info.absoluteDir() != last)
        {
            last = it.fileInfo().absoluteDir();
            query.prepare("INSERT INTO Folders (name) VALUES (?)");
            query.addBindValue(last.dirName());
            query.exec();
            folders->submitAll();
        }
        query.prepare("INSERT INTO Entries (filename, labelId, datasetId, folderId) VALUES (?, ?, ?, ?)");
        query.addBindValue(it.fileInfo().fileName());
        query.addBindValue(0);
        query.addBindValue(datasetId);
        query.addBindValue(folders->rowCount());
        query.exec();
	}
    db->entries()->submitAll();
}
