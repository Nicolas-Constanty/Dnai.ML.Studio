#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

class Project : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString dbPath READ dbPath WRITE setDbPath NOTIFY dbPathChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    QString name() const;
    QString dbPath() const;
    QString path() const;
    Q_INVOKABLE void save();
    Q_INVOKABLE void saveTo(const QString &path);
    Q_INVOKABLE void open(const QString &path);

public slots:
    void setName(QString name);
    void setDbPath(QString dbPath);
    void setPath(QString path);

signals:
void nameChanged(QString name);
void dbPathChanged(QString dbPath);
void pathChanged(QString path);

private:
QString m_name;
QString m_dbPath;
QString m_path;
};

#endif // PROJECT_H
