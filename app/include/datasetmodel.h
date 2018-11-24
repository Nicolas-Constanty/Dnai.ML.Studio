#ifndef DATASET_MODEL_H
#define DATASET_MODEL_H

#include <QVariantMap>
#include <QSqlRelationalTableModel>

class TableModel : public QSqlRelationalTableModel
{
	Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)

public:
    explicit TableModel(QObject * parent = nullptr, const QSqlDatabase& db = QSqlDatabase());

    Q_INVOKABLE int getRoles(const QByteArray &role);
	void generateRoles();

	QHash<int, QByteArray> roleNames() const override;
	QVariant data(const QModelIndex& idx, int role) const override;

    //    Q_INVOKABLE QVariantMap operator[](int) const;
    int count() const;

    Q_INVOKABLE void setFilter(const QString &filter) override;
    Q_INVOKABLE void display();

public slots:
    void setCount(int count);
    void updateCount(QSqlRecord &model);
    void updateCount();

signals:
    void countChanged(int count);

private:
    QHash<int, QByteArray> m_roles;

    void updateMap();
    int m_count;
};

#endif //DATASET_MODEL_H
