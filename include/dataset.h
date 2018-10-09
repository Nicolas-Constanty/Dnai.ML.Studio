#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include "iterableqqmlpropertymap.h"

namespace dnai {
namespace mlstudio {
class Dataset : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(IterableQQmlPropertyMap *labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY(QList<QString> labelNames READ labelNames WRITE setLabelNames NOTIFY labelNamesChanged)
public:
	explicit Dataset(QObject *parent = nullptr);
	explicit Dataset(const QString &path, QObject *parent = nullptr);

    QString name() const;

    QString path() const;

    IterableQQmlPropertyMap *labels();

    QList<QString> labelNames() const;

    Q_INVOKABLE void appendLabel(const QString &label);
    Q_INVOKABLE void removeLabel(const QString &label);
    Q_INVOKABLE void setLabel(int index, const QString &label);
    Q_INVOKABLE QString getLabel(int index);

public slots:
    void setName(QString name);

    void setPath(QString path);

    void setLabels(IterableQQmlPropertyMap * labels);

    void setLabelNames(QList<QString> labelNames);

signals:
    void nameChanged(QString name);

    void pathChanged(QString path);

    void labelsChanged(IterableQQmlPropertyMap * labels);

    void labelNamesChanged(QList<QString> labelNames);

private:
    void initDataset();

    QQmlPropertyMap m_data;
    IterableQQmlPropertyMap m_labels;
    QVector<QString> m_labelNames;

};
}
}

#endif // DATASET_H
