#include "mainwindow.h"
#include "resourcebrowser.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ResourceBrowser rb;
    rb.show();


    return a.exec();
}
