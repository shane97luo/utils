#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

protected:
    void run();

signals:
    void Sig2ThreadSelf();
public slots:
    void SlotDoSomething();

};

#endif // MYTHREAD_H
