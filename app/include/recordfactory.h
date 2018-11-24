#ifndef RECORDFACTORY_H
#define RECORDFACTORY_H

#include <QSqlRecord>
#include <QSqlField>

#include "ifactory.h"

enum RecordType {
    Dataset,
    Entry,
    Folder,
    Label,
    Provider
};

class LabelRecord : public QSqlRecord {
public:
    LabelRecord() : QSqlRecord() {
        append(QSqlField("id", QVariant::Int));
        append(QSqlField("name", QVariant::String));
    }
};

class ProviderRecord : public QSqlRecord {
public:
    ProviderRecord() : QSqlRecord() {
        append(QSqlField("id", QVariant::Int));
        append(QSqlField("name", QVariant::String));
    }
};

class FolderRecord : public QSqlRecord {
public:
    FolderRecord() : QSqlRecord() {
        append(QSqlField("id", QVariant::Int));
        append(QSqlField("name", QVariant::String));
    }
};

class DatasetRecord : public QSqlRecord {
public:
    DatasetRecord() : QSqlRecord() {
        append(QSqlField("id", QVariant::Int));
        append(QSqlField("name", QVariant::String));
        append(QSqlField("path", QVariant::String));
        append(QSqlField("providerId", QVariant::Int));
    }
};

class EntryRecord : public QSqlRecord {
public:
    EntryRecord() : QSqlRecord() {
        append(QSqlField("id", QVariant::Int));
        append(QSqlField("filename", QVariant::String));
        append(QSqlField("labelId", QVariant::Int));
        append(QSqlField("datasetId", QVariant::Int));
        append(QSqlField("folderId", QVariant::Int));
    }
};


class RecordFactory : public IFactory<QSqlRecord, RecordType> {
public:
    RecordFactory() {
        registerObject<DatasetRecord>(RecordType::Dataset);
        registerObject<EntryRecord>(RecordType::Entry);
        registerObject<FolderRecord>(RecordType::Folder);
        registerObject<LabelRecord>(RecordType::Label);
        registerObject<ProviderRecord>(RecordType::Provider);
    }
};

#endif // RECORDFACTORY_H
