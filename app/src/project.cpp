#include <QFileInfo>
#include <QtDebug>
#include <QUrl>
#include <QJsonDocument>

#include "project.h"

Project::Project(QObject *parent) : QObject (parent)
{

}

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
    QFile file(m_path);
    if (!file.open(QFile::WriteOnly))
    {
        qWarning() << "==Project== Unable to open project file " << file.fileName() << ": " << file.errorString();
    }
    file.write(QJsonDocument(serialise()).toJson(QJsonDocument::Compact));
    qDebug() << "==Project== Project saved into " << file.fileName();
    file.close();
}

void Project::saveTo(const QString &path)
{
    m_path = path;
    save();
}

Project *Project::load(const QString &path)
{
    QFileInfo info(QUrl(path).toLocalFile());
    if (!info.exists())
    {
        qWarning() << "File doesn't exist";
        return nullptr;
    }
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
    {
        qWarning() << "==Project== Unable to open project file " << file.fileName() << ": " << file.errorString();
        return nullptr;
    }

    const auto data = file.readAll();
    try {
        QJsonParseError err;
        const QJsonObject obj(QJsonDocument::fromJson(data, &err).object());
        if (err.error != QJsonParseError::NoError)
        {
            qWarning() << "==Project==" << err.errorString() << "at character :" << err.offset;
            file.close();
            return nullptr;
        }
        auto project = new Project();
        project->deserialise(obj);
        project->setPath(info.path());
        project->initDB();
        return project;
    } catch (std::exception &e) {
        Q_UNUSED(e)
        qWarning() << "==Project== Couldn't parse file.";
        return nullptr;
    }
}


Project *Project::defaultProject()
{
    auto project = new Project();
    project->setName("Default project");
    return project;
}

QString Project::description() const
{
    return m_description;
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

void Project::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

void Project::deserialise(const QJsonObject &obj)
{
    m_name = obj["name"].toString();
    m_dbPath = m_path + "/" + m_name + ".db";
}

QJsonObject Project::serialise()
{

    QJsonObject obj;
    obj["name"] = m_name;
    obj["description"] = m_description;
    return obj;
}

void Project::initDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("dnai");
    db.setDatabaseName(m_dbPath);
    db.setUserName("user");
    db.setPassword("dn41");
    if (!db.open()) {
        qWarning() << "Cannot open db!!!";
    }
}
