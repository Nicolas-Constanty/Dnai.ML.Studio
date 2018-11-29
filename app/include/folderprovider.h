#ifndef FOLDERPROVIDER_H
#define FOLDERPROVIDER_H

#include <QDebug>
#include <QVariantList>
#include <iostream>
#include <QFileInfoList>

#include "aprovider.h"
#include "datasetmodel.h"
#include "parsefoldertask.h"

class FolderProvider : public QObject, public AProvider {
    Q_OBJECT
public:
    explicit FolderProvider()
        : QObject(nullptr), AProvider()
	{
    }

    void preload(const QString &path, int datasetId) override;
protected:
    void initModel(const QString& name) override;

};



#endif // FOLDERPROVIDER_H
