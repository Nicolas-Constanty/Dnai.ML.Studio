#include "editor.h"

namespace dnai {
namespace mlstudio {
	Editor::Editor(QObject* parent) : QObject(parent)
	{
	}

    DatasetHandler* Editor::datasetHandler()
	{
		return &m_datasetHandler;
	}
}
}
