#include <QSqlRecord>
#include <QDebug>

#include "datasetmodel.h"

DatasetModel::DatasetModel(QObject* parent, const QSqlDatabase& db) : QSqlTableModel(parent, db)
{
}

void DatasetModel::generateRoles()
{
	m_roles.clear();
	const auto nbCols = this->columnCount();
	for (auto i = 0; i < nbCols; i++)
	{
		m_roles[Qt::UserRole + i + 1] = QVariant(this->headerData(i, Qt::Horizontal).toString()).toByteArray();
	}
}

QHash<int, QByteArray> DatasetModel::roleNames() const
{
	return m_roles;
}

QVariant DatasetModel::data(const QModelIndex& idx, const int role) const
{
	if (idx.row() >= rowCount())
	{
		return QString("");
	}
	if (role < Qt::UserRole)
	{
		return QSqlQueryModel::data(idx, role);
    }
//    qDebug() << "From data ====> " << record(idx.row()).value(0);
//    qDebug() << "From data ====> " << record(idx.row()).value(1);
	return record(idx.row()).value(QString(m_roles[role]));
}
