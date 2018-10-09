#include "editor.h"

namespace dnai {
namespace mlstudio {
	Editor::Editor(QObject* parent) : QObject(parent)
	{
	}

	const DatasetHandler* Editor::datasetHandler() const
	{
		return &m_datasetHandler;
	}
}
}