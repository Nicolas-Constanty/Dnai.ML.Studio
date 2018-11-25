#ifndef FOLDERPROVIDER_H
#define FOLDERPROVIDER_H

#include <QDebug>
#include <QVariantList>
#include <iostream>

#include "aprovider.h"
#include "datasetmodel.h"

class FolderProvider : public AProvider {
public:
    explicit FolderProvider()
        : AProvider()
	{
	}
    void generateModel(const QString &path, int datasetId) override;
    class EntryRecord {
    public:
        QVariantList ids;
        QVariantList filenames;
        QVariantList labelIds;
        QVariantList datasetIds;
        QVariantList folderIds;

        void clear()
        {
            ids.clear();
            filenames.clear();
            labelIds.clear();
            datasetIds.clear();
            folderIds.clear();
            m_count = 0;
        }

        void appendRecord(int id, const QString &filename, int labelId, int datasetId, int folderId)
        {
            ids << id;
            filenames << filename;
            labelIds << labelId;
            datasetIds << datasetId;
            folderIds << folderId;
            m_count++;
        }

        int count()
        {
            return m_count;
        }

        friend QDebug operator<<(QDebug os, const FolderProvider::EntryRecord& e)
        {
            os << e.ids << "\n" << e.filenames << "\n" << e.labelIds << "\n" << e.datasetIds << "\n" << e.folderIds;
            return os;
        }
    private:
        int m_count = 0;
    };

    void preload(const QString &path, int datasetId) override;
protected:
    void initModel(const QString& name) override;
private:
    static bool bulkInsertRequest(EntryRecord &record, TableModel *db);

};



#endif // FOLDERPROVIDER_H
