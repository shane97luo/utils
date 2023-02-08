#include "mainwindow.h"

#include <cups.h>

#include <QApplication>
#include <QDebug>

#include <memory>

#include <QTime>

#include <QDir>

#include <QMetaObject>
#include <QMetaMethod>
#include <iostream>
#include <functional>

#include <QProcess>
#include <QDebug>
#include <QTimer>

void StartProcess()
{
    QProcess* process = new QProcess;
    process->setProgram("/home/shane/untitled/build/untitled_1");
    //process->setProgram("/home/shane/untitled/build/untitled");
    process->setArguments(QStringList() << "dsdsjh");

    process->start();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow win;
    win.show();

    return a.exec();
}
