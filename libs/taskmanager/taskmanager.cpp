#include <QDebug>
#include <QFuture>
#include "taskmanager.h"

TaskManager::TaskManager() :
    m_progress(0)
    , m_indeterminate(true)
    , m_paused(false)
    , m_jobRunning(false)
    , m_canStart(false)
    , m_stoped(true)
{
}

qreal TaskManager::progress() const
{
    return m_progress;
}

QString TaskManager::progressText() const
{
    return m_progressText;
}

QString TaskManager::jobName() const
{
    return m_jobName;
}

bool TaskManager::indeterminate() const
{
    return m_indeterminate;
}

void TaskManager::scheduleTask(Task *task)
{
    if (!task)
        return;
    m_mutex.lock();
    m_tasks.enqueue(task);
    m_mutex.unlock();
}

void TaskManager::pause()
{
    qDebug() << "Start pause";
    setPaused(true);
    emit pauseTask();
}

void TaskManager::resume()
{
    qDebug() << "Resume";
    setPaused(false);
    emit resumeTask();
}

void TaskManager::setStoped(bool stoped)
{
    if (m_stoped == stoped)
        return;
    m_mutex.lock();
    m_stoped = stoped;
    m_mutex.unlock();
    emit stopedChanged(m_stoped);
}

bool TaskManager::jobRunning() const
{
    return m_jobRunning;
}

bool TaskManager::paused() const
{
    return m_paused;
}

bool TaskManager::stoped() const
{
    return m_stoped;
}

void TaskManager::setProgress(qreal progress)
{
    if (qFuzzyCompare(m_progress, progress))
        return;

    m_mutex.lock();
    m_progress = progress;
    m_mutex.unlock();
    emit progressChanged(m_progress);
}

void TaskManager::setProgressText(QString progressText)
{
    if (m_progressText == progressText)
        return;

    m_mutex.lock();
    m_progressText = progressText;
    m_mutex.unlock();
    emit progressTextChanged(m_progressText);
}

void TaskManager::setJobName(QString jobName)
{
    if (m_jobName == jobName)
        return;

    m_mutex.lock();
    m_jobName = jobName;
    m_mutex.unlock();
    emit jobNameChanged(m_jobName);
}

void TaskManager::setIndeterminate(bool indeterminate)
{
    if (m_indeterminate == indeterminate)
        return;

    m_mutex.lock();
    m_indeterminate = indeterminate;
    m_mutex.unlock();
    emit indeterminateChanged(m_indeterminate);
}

void TaskManager::setPaused(bool pause)
{
    if (m_paused == pause)
        return;
    m_mutex.lock();
    m_paused = pause;
    m_mutex.unlock();
    emit pausedChanged(m_paused);
}

void TaskManager::setJobRunning(bool jobRunning)
{
    if (m_jobRunning == jobRunning)
        return;

    m_mutex.lock();
    m_jobRunning = jobRunning;
    m_mutex.unlock();
    emit jobRunningChanged(m_jobRunning);
}

void TaskManager::taskFinished()
{
    m_mutex.lock();
    m_canStart = true;
    m_mutex.unlock();
    if (m_tasks.empty())
        setJobRunning(false);
    setProgress(1);
    qDebug() << jobName() << "has finished.";
}

void TaskManager::stop()
{
    setStoped(true);
    while (!m_tasks.empty()) {
        delete m_tasks.dequeue();
    }
}

void TaskManager::run()
{
    setStoped(false);
    m_mutex.lock();
    m_canStart = true;
    m_mutex.unlock();
    while (isRunning() && !m_stoped) {
        if (m_paused)
        {
            QThread::msleep(100);
            continue;
        }
        if (!m_tasks.empty() && m_canStart)
        {
            m_mutex.lock();
            auto task = m_tasks.dequeue();
            QObject::connect(task, SIGNAL(isFinished()),
                             this, SLOT(taskFinished()));
            QObject::connect(this, SIGNAL(pauseTask()),
                             task, SLOT(pause()));
            QObject::connect(this, SIGNAL(resumeTask()),
                             task, SLOT(resume()));
            auto progressTask = dynamic_cast<ProgressTask *>(task);
            if (progressTask)
            {
                QObject::connect(progressTask, SIGNAL(progressChanged(qreal)),
                                 this, SLOT(setProgress(qreal)));
                QObject::connect(progressTask, SIGNAL(progressTextChanged(const QString&)),
                                 this, SLOT(setProgressText(const QString &)));
                QObject::connect(progressTask, SIGNAL(indeterminateChanged(bool)),
                                 this, SLOT(setIndeterminate(bool)));
            }
            QThreadPool::globalInstance()->start(task, task->priority());
            m_canStart = false;
            m_mutex.unlock();
            setJobName(task->name());
            setJobRunning(true);
            setIndeterminate(progressTask == nullptr);
        }
        QThread::usleep(100);
    }
}
