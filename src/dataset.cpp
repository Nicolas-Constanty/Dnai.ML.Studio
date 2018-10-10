#include "dataset.h"


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

void Dataset::initDataset()
{
}

