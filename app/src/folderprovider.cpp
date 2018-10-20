#include <QDirIterator>
#include <QSqlRecord>
#include "folderprovider.h"

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

void FolderProvider::generateModel()
{
	QFileInfo infos(m_path);
	if (!infos.isDir())
		return;

	QDirIterator it(m_path, QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		QFile f(it.next());
		QSqlRecord rec;
		rec.setValue("Filename", f.fileName());
		m_data.insertRecord(-1, rec);
	}
}
