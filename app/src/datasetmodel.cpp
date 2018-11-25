#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
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
    auto x = Qt::UserRole + 1;
	for (auto i = 0; i < nbCols; i++)
    {
        m_roles[x++] = QVariant(this->headerData(i, Qt::Horizontal).toString()).toByteArray();
    }
    for (auto i = 0; i < nbCols; i++)
    {
        m_roles[x++] = QVariant(QString("_" + this->headerData(i, Qt::Horizontal).toString())).toByteArray();
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
    auto count = role - (Qt::UserRole + 1);

    if (relation(count).isValid())
    {
        return record(idx.row()).value(count);
    }

    if (m_roles.value(role).at(0) == '_')
    {
        auto m = role - columnCount() - (Qt::UserRole + 1);

        if (relation(m).isValid())
        {
            QSqlQuery q;
            q.prepare("SELECT id FROM " + tableName() + "  WHERE id=" + QString::number(idx.row()));
            if (!q.exec())
                qDebug() << q.lastError();
            return q.record().value(0);
        }
    }
    return record(idx.row()).value(QString(m_roles.value(role)));
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
    qDebug()  << "==App.DatasetModel== " << rowCount();
    for (auto i = 0; i < rowCount(); i++)
        qDebug()  << "==App.DatasetModel== " << "Rec " << i << " : " << record(i);
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
