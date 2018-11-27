#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QThread>
#include <QThreadPool>
#include <QMutex>
#include <QQueue>

#include "taskmanager_global.h"
#include "task.h"
#include "progresstask.h"

class TASKMANAGERSHARED_EXPORT TaskManager : public QThread
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString progressText READ progressText WRITE setProgressText NOTIFY progressTextChanged)
    Q_PROPERTY(QString jobName READ jobName WRITE setJobName NOTIFY jobNameChanged)
    Q_PROPERTY(bool indeterminate READ indeterminate WRITE setIndeterminate NOTIFY indeterminateChanged)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(bool stoped READ stoped WRITE setStoped NOTIFY stopedChanged)
    Q_PROPERTY(bool jobRunning READ jobRunning WRITE setJobRunning NOTIFY jobRunningChanged)

public:
    TaskManager();

    qreal progress() const;
    QString progressText() const;
    QString jobName() const;
    bool indeterminate() const;

    void scheduleTask(Task *);



    bool jobRunning() const;

    bool paused() const;

    bool stoped() const;

public slots:
    void setProgress(qreal progress);
    void setProgressText(QString progressText);
    void setJobName(QString jobName);
    void setIndeterminate(bool indeterminate);

    void setJobRunning(bool jobRunning);
    void taskFinished();

    void stop();
    void pause();
    void resume();


signals:
    void progressChanged(qreal progress);
    void progressTextChanged(QString progressText);
    void jobNameChanged(QString jobName);
    void indeterminateChanged(bool indeterminate);
    void pausedChanged(bool pause);

    void jobRunningChanged(bool jobRunning);

    void pauseTask();
    void resumeTask();

    void stopedChanged(bool stoped);

protected:
    // QThread interface
    void run();
    void setPaused(bool pause);
    void setStoped(bool stoped);

private:
    qreal m_progress;
    bool m_indeterminate;
    bool m_paused;
    bool m_jobRunning;
    bool m_canStart;
    QString m_progressText;
    QString m_jobName;
    QMutex m_mutex;
    QQueue<Task *> m_tasks;
    bool m_stoped;
};

#endif // TASKMANAGER_H
