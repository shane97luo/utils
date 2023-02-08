#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QSaveFile>
#include <QTimer>
#include <QElapsedTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setWindowTitle("Jimmy");

    QDBusConnection::sessionBus().connect(  ""
                                            , kPath
                                            , kInterface
                                            , kName
                                            , this
                                            , SLOT(ActiveEvent(int)));


    QDBusConnection dbus_connect_ = QDBusConnection::sessionBus();

    if(!dbus_connect_.registerService(kServiceOn))
    {
        qDebug() << "dbus_connect_ error:" << dbus_connect_.lastError().message();
    }

    dbus_connect_.registerObject(kObj1, this, QDBusConnection::ExportAllSlots);


    connect(this, &MainWindow::SigTry, this, &MainWindow::SlotSigTry);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Park(int a)
{
    qDebug() << Q_FUNC_INFO << a;
}

constexpr int kTimes = 5;
void MainWindow::on_pushButton_clicked()
{

    static int i = 0;

    QDBusMessage msg =QDBusMessage::createSignal("/qqq",  "heygears.interface", "checkIn");
    msg << i;

    QDBusConnection::sessionBus().send(msg);

    i++;
    qDebug() << "on_pushButton_clicked" << i;
}

void MainWindow::on_pushButton_2_clicked()
{

    qDebug() << "on_pushButton_2_clicked" ;
}

void MainWindow::on_pushButton_3_clicked()
{
    QElapsedTimer timer;
    timer.start();

    for (int i=0;i<kTimes ;i++ ) {

    }

    qDebug() << "on_pushButton_3_clicked" << timer.elapsed() << "seconds";
}

void MainWindow::ActiveEvent(int a)
{
    qDebug() << Q_FUNC_INFO << a;
    ui->pushButton->setText(QString::number(a));
}

int MainWindow::PubEvent(int a)
{
    qDebug() << Q_FUNC_INFO << a;
    return a;
}

void MainWindow::SlotSigTry(QString str)
{
    qDebug() << Q_FUNC_INFO << str;
}

void MainWindow::OnDownloadSlice(int a)
{
    qDebug() << Q_FUNC_INFO << a;
    QThread::sleep(4);
    qDebug() << Q_FUNC_INFO << "wake";

    return;
}
