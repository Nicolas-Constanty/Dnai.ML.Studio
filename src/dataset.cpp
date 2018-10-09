#include "dataset.h"

namespace dnai {
namespace mlstudio {
	Dataset::Dataset(QObject* parent) : QObject(parent)
	{
	}

	Dataset::Dataset(const QString& path, QObject* parent) : QObject(parent)
	{
		m_data["path"] = path;
	}

	QString Dataset::name() const
	{
		return  m_data["name"].toString();
	}

	QString Dataset::path() const
	{
		return  m_data["path"].toString();
	}

	IterableQQmlPropertyMap* Dataset::labels()
	{
		return &m_labels;
	}

	QList<QString> Dataset::labelNames() const
	{
		return m_labelNames.toList();
	}

	void Dataset::appendLabel(const QString& label)
	{
		if (m_labelNames.indexOf(label) == m_labelNames.count())
		{
			m_labelNames.append(label);
		}
		emit labelNamesChanged(labelNames());
	}

	void Dataset::removeLabel(const QString& label)
	{
		if (m_labelNames.indexOf(label) != m_labelNames.count())
		{
			m_labelNames.removeOne(label);
		}
		emit labelNamesChanged(labelNames());
	}

	void Dataset::setLabel(int index, const QString& label)
	{
        m_labelNames[index] = label;
		emit labelNamesChanged(labelNames());
	}

	QString Dataset::getLabel(const int index)
	{
		return m_labelNames[index];
	}

	void Dataset::setName(const QString name)
	{
		m_data["name"] = name;
		emit nameChanged(name);
	}

	void Dataset::setPath(const QString path)
	{
		m_data["path"] = path;
		emit pathChanged(path);
	}

	void Dataset::setLabels(IterableQQmlPropertyMap* labels)
	{
        for (const auto &key : (*labels).keys())
			m_labels[key] = (*labels)[key];
		emit labelsChanged(&m_labels);
	}

	void Dataset::setLabelNames(QList<QString> labelNames)
	{
		for (const auto& l : labelNames)
		{
			if (m_labelNames.indexOf(l) == m_labelNames.count())
			{
				m_labelNames.append(l);
			}
		}
	}

	void Dataset::initDataset()
	{
	}
}
}
