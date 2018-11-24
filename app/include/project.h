#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QJsonObject>
#include <QSqlDatabase>

class Project : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString dbPath READ dbPath WRITE setDbPath NOTIFY dbPathChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

public:
    Project(QObject *parent = nullptr);
    QString name() const;
    QString dbPath() const;
    QString path() const;
    Q_INVOKABLE void save();
    Q_INVOKABLE void saveTo(const QString &path);
    Q_INVOKABLE static Project *load(const QString &path);
    Q_INVOKABLE static Project *defaultProject();

    QString description() const;
    Project &operator=(const Project &proj);

public slots:
    void setName(const QString &name);
    void setDbPath(const QString &dbPath);
    void setPath(const QString &path);

    void setDescription(const QString &description);

signals:
void nameChanged(QString name);
void dbPathChanged(QString dbPath);
void pathChanged(QString path);

void descriptionChanged(QString description);

private:
void deserialise(const QJsonObject &obj);
QJsonObject serialise();
void initDB();
QString filename();

QString m_name;
QString m_dbPath;
QString m_path;
QSqlDatabase m_db;
QString m_description;
};

bool operator==(const Project& lhs, const Project& rhs);

#endif // PROJECT_H
