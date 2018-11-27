#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QMutex>
#include <QRunnable>

#include "taskmanager_global.h"

class TASKMANAGERSHARED_EXPORT Task : public QObject, public QRunnable
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool isRunning READ isRunning WRITE setIsRunning NOTIFY isRunningChanged)

public:
    Task(QRunnable *runnable, const QString &name, int priority = 0);
    virtual ~Task();
    int priority() const;
    const QString &name() const;
    virtual void run();
    bool isRunning() const;

public slots:
    void setIsRunning(bool isRunning);
    void pause();
    void resume();

signals:
    void isRunningChanged(bool isRunning);
    void isFinished();

private:
    const QString m_taskName;
    int m_priority;
    QRunnable *m_runnable;
    bool m_isRunning;
    bool m_isFinished;
    bool m_paused;
    QString m_name;

protected:
    QMutex m_mutex;

};

class TASKMANAGERSHARED_EXPORT QRunnableProgress :  public QObject, public QRunnable
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString progressText READ progressText WRITE setProgressText NOTIFY progressTextChanged)
    Q_PROPERTY(bool indeterminate READ indeterminate WRITE setIndeterminate NOTIFY indeterminateChanged)

public:
    QRunnableProgress() : QObject() {}
    qreal progress() const;
    QString progressText() const;

    bool indeterminate() const;

public slots:
    void setProgress(qreal progress);
    void setProgressText(const QString &progressText);

    void setIndeterminate(bool indeterminate);

signals:
    void progressChanged(qreal progress);
    void progressTextChanged(const QString &progressText);

    void indeterminateChanged(bool indeterminate);

private:
    qreal m_progress;
    QString m_progressText;

    bool m_indeterminate;
};

#endif // TASK_H
