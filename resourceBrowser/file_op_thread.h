#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QMutex>

class FileOpThread : public QThread
{
    Q_OBJECT
public:
    explicit FileOpThread(QObject *parent = nullptr);

protected:
    void run();

private:
    QMutex m_locker;
    bool b_can_run;

signals:
    void Sig2ThreadSelf();

public slots:

    void SlotStopErgenmercy();

    void SlotDoSomething();

};

#endif // MYTHREAD_H
