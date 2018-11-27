#ifndef PROGRESSTASK_H
#define PROGRESSTASK_H

#include "task.h"

class TASKMANAGERSHARED_EXPORT ProgressTask : public Task
{
    Q_OBJECT
    Q_PROPERTY(qreal progress READ progress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(QString progressText READ progressText WRITE setProgressText NOTIFY progressTextChanged)
    Q_PROPERTY(bool indeterminate READ indeterminate WRITE setIndeterminate NOTIFY indeterminateChanged)
public:
    ProgressTask(QRunnableProgress *runnable, const QString &name, int priority = 0);
    qreal progress() const;
    const QString &progressText() const;

    bool indeterminate() const;

public slots:
    void setProgress(qreal progress);
    void setProgressText(const QString &progressText);
    void setMaxPercent();

    void setIndeterminate(bool indetermined);

signals:
    void progressChanged(qreal progress);
    void progressTextChanged(const QString &progressText);

    void indeterminateChanged(bool indetermined);

private:
    qreal m_progress;
    bool m_indeterminate;
    QString m_progressText;
};

#endif // PROGRESSTASK_H
