#include <QFileInfo>
#include <QSqlRecord>
#include "aprovider.h"

const QSqlTableModel& AProvider::data() const
{
	return m_data;
}

QString AProvider::toCsv()
{
	QString textData;
	const auto rows = m_data.rowCount();
	const auto columns = m_data.columnCount();

	for (auto i = 0; i < rows; i++)
	{
		for (auto j = 0; j < columns; j++)
		{
			textData += m_data.data(m_data.index(i, j)).toString();
			textData += ",";
		}
		textData += "\n";
	}

	return textData;
}

void AProvider::updateProvider(const QString& name)
{
	initModel(name);
//	generateModel();
}

void AProvider::updateProviderFromCsv(const QString& name, const QString& csvPath)
{
	initModel(name);
	generateModelFromCsv(csvPath);
}

void AProvider::generateModelFromCsv(const QString& path)
{
	QFile csv(path);
	csv.open(QIODevice::ReadOnly);

	auto csvData = QString(csv.readAll());
	auto rows = csvData.split("\n");
	for (const auto& r : rows)
	{
		auto columns = r.split(",");
		QSqlRecord rec;
		auto i = 0;
		for (const auto& c : columns)
		{
			rec.setValue(i++, c);
		}
		m_data.insertRecord(-1, rec);
	}
}
