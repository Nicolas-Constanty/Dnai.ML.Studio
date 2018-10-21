#include "editor.h"

Editor *Editor::m_instance = nullptr;
Editor::Editor(QObject* parent) : QObject(parent)
{
	if (!m_instance)
		m_instance = this;
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

void Editor::setProject(Project *project)
{
    if (m_project == project)
        return;

    m_project = project;
    emit projectChanged(m_project);
}
