#include "mainwindow.h"

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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow win;
    win.show();

    return a.exec();
}
