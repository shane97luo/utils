#include "mainwindow.h"

#include <QDebug>
#include "file_op_thread.h"

#include "resourcebrowser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    FileOpThread *myThread = new FileOpThread;

    connect(this, &MainWindow::Sig2Thread, myThread, &FileOpThread::SlotDoSomething/*, Qt::DirectConnection*/);

    myThread->SlotDoSomething();

    emit Sig2Thread();


    myThread->start();
    qDebug()<<"fun [MainWindow] cur thread id: "<<QThread::currentThread();


}

MainWindow::~MainWindow()
{
}

