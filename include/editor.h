#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include "datasethandler.h"

namespace dnai {
namespace mlstudio {

class Editor : public QObject
{
	Q_OBJECT
    Q_PROPERTY(const dnai::mlstudio::DatasetHandler *datasetHandler READ datasetHandler CONSTANT)

public:
	explicit Editor(QObject *parent = nullptr);

public slots:
	const DatasetHandler *datasetHandler() const;

private:
	DatasetHandler m_datasetHandler;
};
}
}

#endif // EDITOR_H
