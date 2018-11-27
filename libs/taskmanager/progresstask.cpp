#include "progresstask.h"

ProgressTask::ProgressTask(QRunnableProgress *runnable, const QString &name, int priority) :
    Task(runnable, name, priority),
    m_progress(0),
    m_indeterminate(false),
    m_progressText("")
{
    QObject::connect(runnable, SIGNAL(progressChanged(qreal)),
                     this, SLOT(setProgress(qreal)));
    QObject::connect(runnable, SIGNAL(progressTextChanged(const QString&)),
                     this, SLOT(setProgressText(const QString&)));
    QObject::connect(this, SIGNAL(isFinished()),
                     this, SLOT(setMaxPercent()));
    QObject::connect(runnable, SIGNAL(indeterminateChanged(bool)),
                     this, SLOT(setIndeterminate(bool)));
}

qreal ProgressTask::progress() const
{
    return m_progress;
}

const QString &ProgressTask::progressText() const
{
    return m_progressText;
}

void ProgressTask::setProgress(qreal progress)
{
    if (qFuzzyCompare(m_progress, progress))
        return;
    m_mutex.lock();
    m_progress = progress;
    m_mutex.unlock();
    emit progressChanged(m_progress);
}

void ProgressTask::setProgressText(const QString &progressText)
{
    if (progressText == m_progressText)
        return;
    m_mutex.lock();
    m_progressText = progressText;
    m_mutex.unlock();
    emit progressTextChanged(m_progressText);
}

void ProgressTask::setMaxPercent()
{
    setProgress(1);
}

void ProgressTask::setIndeterminate(bool indeterminate)
{
    if (m_indeterminate == indeterminate)
        return;

    m_mutex.lock();
    m_indeterminate = indeterminate;
    m_mutex.unlock();
    emit indeterminateChanged(m_indeterminate);
}

bool ProgressTask::indeterminate() const
{
    return m_indeterminate;
}
