#include <QSqlField>

#include "dataset.h"
#include "datasethandler.h"

Dataset::Dataset(QObject* parent) : QObject(parent), m_provider(nullptr)
{
	
}

Dataset::Dataset(AProvider &provider, QObject* parent) : QObject(parent), m_provider(&provider)
{
}

void Dataset::initFromRecord(QSqlRecord *record)
{
    for (auto i = 0; i < record->count(); i++)
    {
        auto field = record->field(i);
        m_data[field.name()] = field.value();
    }
}

QString Dataset::name() const
{
    return  m_data["name"].toString();
}

QString Dataset::path() const
{
    return "";//m_provider->path();
}

IterableQQmlPropertyMap* Dataset::labels()
{
    return &m_labels;
}

void Dataset::setName(const QString& name)
{
    m_data["name"] = name;
	m_provider->updateProvider(name);
    emit nameChanged(name);
}

void Dataset::setPath(const QString& path)
{
//	m_provider->setPath(path);
	const auto temp = &m_provider;
	m_provider = DatasetHandler::createProvider(path);
	m_provider->updateProvider(name());
	delete temp;
	emit pathChanged(path);
}

void Dataset::setLabels(IterableQQmlPropertyMap* labels)
{
    for (const auto &key : (*labels).keys())
        m_labels[key] = (*labels)[key];
    emit labelsChanged(&m_labels);
}

const QSqlTableModel& Dataset::data() const
{
	return m_provider->data();
}

void Dataset::initDataset()
{
}

