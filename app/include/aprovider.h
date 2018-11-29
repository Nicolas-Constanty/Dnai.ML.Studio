#ifndef IPROVIDER_H
#define IPROVIDER_H

#include <QSqlTableModel>

class AProvider {
public:
    enum Type : int
    {
        DIR = 1,
        MP4,
        CSV,
        INVALID
    };
    explicit AProvider() : m_data(nullptr, QSqlDatabase::database()), m_init(false)
	{
	}

    virtual ~AProvider() = default;

    virtual const QSqlTableModel& data() const;
    virtual QString toCsv();

	void updateProvider(const QString &name);
	void updateProviderFromCsv(const QString &name, const QString &csvPath);

    virtual void preload(const QString &path, int datasetId) = 0;

protected:
	virtual void initModel(const QString& name) = 0;
    void generateModelFromCsv(const QString& path);

protected:
    QSqlTableModel m_data;
	bool m_init;
};

#endif // IPROVIDER_H
