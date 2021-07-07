#include "file_op_thread.h"

#include <QDebug>
//#include <QMutexLocker>

FileOpThread::FileOpThread(QObject *parent) : QThread(parent)
{
    connect(this, &FileOpThread::Sig2ThreadSelf, this, &FileOpThread::SlotDoSomething, Qt::DirectConnection);

}

void FileOpThread::run()
{
    QMutexLocker locker(&m_locker);
    b_can_run = true;
    locker.unlock();

    qDebug()<<"fun [run] cur thread id: "<<QThread::currentThread();

    // �����˳��߳�
    QMutexLocker lockerc(&m_locker);
    if( !b_can_run)
        return;

    emit Sig2ThreadSelf();
}

/* ����ֹͣ�߳�*/
void FileOpThread::SlotStopErgenmercy()
{
    QMutexLocker locker(&m_locker);
    b_can_run = false;

}

void FileOpThread::SlotDoSomething()
{
    qDebug()<<"fun [SlotDoSomething] cur thread id: "<<QThread::currentThread();
}
