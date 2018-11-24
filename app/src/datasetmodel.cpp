#include <QSqlRecord>
#include <QDebug>

#include "datasetmodel.h"

TableModel::TableModel(QObject* parent, const QSqlDatabase& db) : QSqlRelationalTableModel(parent, db)
{
    m_count = rowCount();
}

int TableModel::getRoles(const QByteArray &role)
{
    return m_roles.key(role);
}

void TableModel::generateRoles()
{
    m_roles.clear();
	const auto nbCols = this->columnCount();
	for (auto i = 0; i < nbCols; i++)
	{
        m_roles[Qt::UserRole + i + 1] = QVariant(this->headerData(i, Qt::Horizontal).toString()).toByteArray();
    }
}

QHash<int, QByteArray> TableModel::roleNames() const
{
	return m_roles;
}

QVariant TableModel::data(const QModelIndex& idx, const int role) const
{
	if (idx.row() >= rowCount())
	{
		return QString("");
	}
    if(role < Qt::UserRole)
    {
        return QSqlQueryModel::data(idx, role);
    }
    // search for relationships
    for (int i = 0; i < columnCount(); i++)
    {
        if (this->relation(i).isValid())
        {
//                qDebug() << idx.row() << role << record(idx.row()).value(QString(m_roles.value(role)));
            return record(idx.row()).value(QString(m_roles.value(role)));
        }
    }

    // if no valid relationship was found
    return QSqlQueryModel::data(this->index(idx.row(), role - Qt::UserRole - 1), Qt::DisplayRole);
}

int TableModel::count() const
{
    return m_count;
}

void TableModel::setFilter(const QString &filter)
{
    QSqlRelationalTableModel::setFilter(filter);
}

void TableModel::display()
{
    qDebug() << rowCount();
    for (auto i = 0; i < rowCount(); i++)
        qDebug() << "Rec " << i << " : " << record(i);
}

void TableModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(m_count);
}

void TableModel::updateCount(QSqlRecord &model)
{
    qDebug() << "Update count";
    Q_UNUSED(model);
    setCount(rowCount());
}

void TableModel::updateCount()
{
    setCount(rowCount());
}

// TODO : Optimize this later
//QVariantMap DatasetModel::operator[](int row) const
//{
//    QHash<int,QByteArray> names = roleNames();
//    QHashIterator<int, QByteArray> i(names);
//    QVariantMap res;
//    while (i.hasNext()) {
//        i.next();
//        QModelIndex idx = index(row, 0);
//        QVariant data = idx.data(i.key());
//        res[i.value()] = data;
//    }
//    return res;
//}
