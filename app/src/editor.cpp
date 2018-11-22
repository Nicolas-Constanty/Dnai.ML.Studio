#include "editor.h"

Editor *Editor::m_instance = nullptr;
Editor::Editor(QObject* parent) : QObject(parent)
{
	if (!m_instance)
		m_instance = this;
    m_project = Project::defaultProject();
    m_datasetHandler.initDatabaseHandler();
}

DatasetHandler* Editor::datasetHandler()
{
    return &m_datasetHandler;
}

ApiHandler *Editor::apiHandler()
{
    return  &m_apiHandler;
}

Editor& Editor::instance()
{
	return *m_instance;
}

Project *Editor::project() const
{
    return m_project;
}

bool Editor::loadProject(const QString &path)
{
    auto p = Project::load(path);
    if (!p)
        return false;
    if (m_project)
    {
        auto t = m_project;
        setProject(p);
        delete t;
    }
    else
        setProject(p);
    return true;
}

void Editor::saveProject()
{
    m_project->save();
}

DatabaseHandler *Editor::databaseHandler()
{
    return &m_databaseHandler;
}

void Editor::setProject(Project *project)
{
    if (m_project == project)
        return;

    m_project = project;
    emit projectChanged(m_project);
}
