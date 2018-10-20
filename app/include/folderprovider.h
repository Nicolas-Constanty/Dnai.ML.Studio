#ifndef FOLDERPROVIDER_H
#define FOLDERPROVIDER_H

#include "aprovider.h"

class FolderProvider : public AProvider {
public:
	explicit FolderProvider(const QString& path)
		: AProvider(path)
	{
	}

protected:
	void generateModel() override;
	void initModel(const QString& name) override;
};

#endif // FOLDERPROVIDER_H
