#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include "iterableqqmlpropertymap.h"
#include "aprovider.h"

class Dataset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(IterableQQmlPropertyMap *labels READ labels WRITE setLabels NOTIFY labelsChanged)
public:
    enum Type : uint
	{
		DIR,
		MP4,
		CSV,
		INVALID
	};
protected:
    explicit Dataset(QObject* parent = nullptr);
public:
    explicit Dataset(AProvider &provider, QObject *parent = nullptr);

    QString name() const;

    QString path() const;

    IterableQQmlPropertyMap *labels();

    Q_INVOKABLE const QSqlTableModel &data() const;


public slots:
    void setName(const QString& name);

    void setPath(const QString& path);

    void setLabels(IterableQQmlPropertyMap * labels);


signals:
    void nameChanged(QString name);

    void pathChanged(QString path);

    void labelsChanged(IterableQQmlPropertyMap * labels);


private:
    void initDataset();

    QQmlPropertyMap m_data;
    IterableQQmlPropertyMap m_labels;
	AProvider *m_provider;
};

#endif // DATASET_H
