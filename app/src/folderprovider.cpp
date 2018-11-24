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
    QDir last;
    auto db = Editor::instance().databaseHandler();
    auto folders = db->folders();
    auto entries = db->entries();
	while (it.hasNext())
	{
        it.next();
        auto info = it.fileInfo();
        if (info.absoluteDir() == path)
            continue;
        if (info.absoluteDir() != last)
        {
            last = it.fileInfo().absoluteDir();
            auto rec = db->recordFactory().create(RecordType::Folder);
            rec->setValue("name", last.dirName());
            folders->insertRecord(-1, *rec);
            folders->submitAll();
            delete rec;
        }
        auto rec = db->recordFactory().create(RecordType::Entry);
        rec->setValue("filename", it.fileInfo().fileName());
        rec->setValue("labelId", 0);
        rec->setValue("datasetId", datasetId);
        rec->setValue("folderId", folders->rowCount());
        entries->insertRecord(-1, *rec);
        delete rec;
    }
    db->entries()->submitAll();
}
