#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include "iterableqqmlpropertymap.h"

class Dataset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(IterableQQmlPropertyMap *labels READ labels WRITE setLabels NOTIFY labelsChanged)
public:
	explicit Dataset(QObject *parent = nullptr);
	explicit Dataset(const QString &path, QObject *parent = nullptr);

    QString name() const;

    QString path() const;

    IterableQQmlPropertyMap *labels();

public slots:
    void setName(QString name);

    void setPath(QString path);

    void setLabels(IterableQQmlPropertyMap * labels);


signals:
    void nameChanged(QString name);

    void pathChanged(QString path);

    void labelsChanged(IterableQQmlPropertyMap * labels);


private:
    void initDataset();

    QQmlPropertyMap m_data;
    IterableQQmlPropertyMap m_labels;

};

#endif // DATASET_H
