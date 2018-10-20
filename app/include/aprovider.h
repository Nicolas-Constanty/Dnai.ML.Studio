#ifndef IPROVIDER_H
#define IPROVIDER_H

#include <QSqlTableModel>

class AProvider {
public:
	explicit AProvider(const QString &path) : m_data(nullptr, QSqlDatabase::database()), m_path(path), m_init(false)
	{
	}

    virtual ~AProvider() = default;

    virtual const QSqlTableModel& data() const;
	virtual QString toCsv();
	const QString& path() const;
	void setPath(const QString& path);

	void updateProvider(const QString &name);
	void updateProviderFromCsv(const QString &name, const QString &csvPath);

protected:
	virtual void initModel(const QString& name) = 0;
	virtual void generateModel() = 0;
	void generateModelFromCsv(const QString& path);

protected:
    QSqlTableModel m_data;
	QString m_path;
	bool m_init;
};

#endif // IPROVIDER_H
