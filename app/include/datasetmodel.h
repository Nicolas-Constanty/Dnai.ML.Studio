#ifndef DATASET_MODEL_H
#define DATASET_MODEL_H

#include <QSqlTableModel>

class DatasetModel : public QSqlTableModel
{
	Q_OBJECT
public:
	explicit DatasetModel(QObject * parent = nullptr, const QSqlDatabase& db = QSqlDatabase());

	void generateRoles();

	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex& idx, int role) const override;
private:
	QHash<int, QByteArray> m_roles;

};

#endif //DATASET_MODEL_H