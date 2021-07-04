#include "mythread.h"

#include <QDebug>

MyThread::MyThread(QObject *parent) : QThread(parent)
{
    connect(this, &MyThread::Sig2ThreadSelf, this, &MyThread::SlotDoSomething,Qt::DirectConnection);

    emit Sig2ThreadSelf();
}

void MyThread::run()
{
    qDebug()<<"fun [run] cur thread id: "<<QThread::currentThread();

    emit Sig2ThreadSelf();
}

void MyThread::SlotDoSomething()
{
    qDebug()<<"fun [SlotDoSomething] cur thread id: "<<QThread::currentThread();
}
