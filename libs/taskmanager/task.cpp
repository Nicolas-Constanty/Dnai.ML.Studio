#include <QThread>
#include "task.h"

Task::Task(QRunnable *runnable, const QString &name, int priority) : QObject (),
    m_taskName(name),
    m_priority(priority),
    m_runnable(runnable),
    m_isRunning(false),
    m_isFinished(false),
    m_paused(false)
{}

Task::~Task() {

}

int Task::priority() const {
    return m_priority;
}

const QString &Task::name() const {
    return m_taskName;
}

void Task::run()
{
    setIsRunning(true);
    m_runnable->run();
    setIsRunning(false);
    emit isFinished();
}

bool Task::isRunning() const
{
    return m_isRunning;
}

void Task::setIsRunning(bool isRunning)
{
    if (m_isRunning == isRunning)
        return;
    m_mutex.lock();
    m_isRunning = isRunning;
    m_mutex.unlock();
    emit isRunningChanged(m_isRunning);
}

void Task::pause()
{
    m_mutex.lock();
    m_paused = true;
    m_mutex.unlock();
}

void Task::resume()
{
    m_mutex.lock();
    m_paused = false;
    m_mutex.unlock();
}

qreal QRunnableProgress::progress() const
{
    return m_progress;
}

QString QRunnableProgress::progressText() const
{
    return m_progressText;
}

void QRunnableProgress::setProgress(qreal progress)
{
    if (qFuzzyCompare(m_progress, progress))
        return;

    m_progress = progress;
    emit progressChanged(m_progress);
}

void QRunnableProgress::setProgressText(const QString &progressText)
{
    if (m_progressText == progressText)
        return;

    m_progressText = progressText;
    emit progressTextChanged(m_progressText);
}

void QRunnableProgress::setIndeterminate(bool indeterminate)
{
    if (m_indeterminate == indeterminate)
        return;

    m_indeterminate = indeterminate;
    emit indeterminateChanged(m_indeterminate);
}

bool QRunnableProgress::indeterminate() const
{
    return m_indeterminate;
}
