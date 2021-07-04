#include "mainwindow.h"

#include <QDebug>
#include "mythread.h"

#include "resourcebrowser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MyThread *myThread = new MyThread;

    connect(this, &MainWindow::Sig2Thread, myThread, &MyThread::SlotDoSomething);

    myThread->SlotDoSomething();

//    emit Sig2Thread();


    myThread->start();
    qDebug()<<"fun [MainWindow] cur thread id: "<<QThread::currentThread();


}

MainWindow::~MainWindow()
{
}

