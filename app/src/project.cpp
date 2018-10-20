#include "project.h"

QString Project::name() const
{
    return m_name;
}

QString Project::dbPath() const
{
    return m_dbPath;
}

QString Project::path() const
{
    return m_path;
}

void Project::save()
{

}

void Project::saveTo(const QString &path)
{

}

void Project::open(const QString &path)
{

}

void Project::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void Project::setDbPath(QString dbPath)
{
    if (m_dbPath == dbPath)
        return;

    m_dbPath = dbPath;
    emit dbPathChanged(m_dbPath);
}

void Project::setPath(QString path)
{
    if (m_path == path)
        return;

    m_path = path;
    emit pathChanged(m_path);
}
