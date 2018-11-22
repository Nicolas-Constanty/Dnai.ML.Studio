#include <QFileInfo>
#include <QtDebug>
#include <QUrl>
#include <QJsonDocument>
#include <QDir>
#include "editor.h"
#include "datasethandler.h"
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
    QFile file(filename());
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "==Project== Unable to open project file " << file.fileName() << ": " << file.errorString();
        file.close();
        return;
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
    const auto p = QUrl(path).toLocalFile();
    QFileInfo info(p);
    if (!info.exists())
    {
        qWarning() << "File doesn't exist";
        return nullptr;
    }
    QFile file(p);
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
        project->setPath(info.absolutePath());
        project->deserialise(obj);
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

void Project::operator=(const Project &proj)
{
    m_name = proj.name();
    m_description = proj.description();
    m_dbPath = proj.dbPath();
    m_path = proj.path();
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
    QFileInfo i(path);
    if(!i.exists())
    {
        m_path = QUrl(path).toLocalFile();
    }
    else
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
    m_description = obj["description"].toString();
    qDebug() << "Deserialise" << m_path;
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
    Editor::instance().databaseHandler()->initDatabases(m_dbPath);
}

QString Project::filename()
{
    return m_path + "/" + m_name + ".mlstd";
}

bool operator==(const Project& lhs, const Project& rhs) {
    return lhs.name() == rhs.name()
            && lhs.name() == rhs.name()
            && lhs.path() == rhs.path()
            && lhs.dbPath() == rhs.dbPath()
            && lhs.description() == rhs.description();
}
