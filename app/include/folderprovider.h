#ifndef FOLDERPROVIDER_H
#define FOLDERPROVIDER_H

#include "aprovider.h"

class FolderProvider : public AProvider {
public:
    explicit FolderProvider()
        : AProvider()
	{
	}
    void generateModel(const QString &path, int datasetId) override;

protected:
	void initModel(const QString& name) override;
};

#endif // FOLDERPROVIDER_H
