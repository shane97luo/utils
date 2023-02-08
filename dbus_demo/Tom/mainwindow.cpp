#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QSaveFile>
#include <QTimer>
#include <QElapsedTimer>

#include <cups.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Tom");

    QDBusConnection::sessionBus().connect( QString() // service
                                           , kPath
                                           , kInterface
                                           , kName
                                           , this
                                           , SLOT(ActiveEvent(int)));

    QDBusConnection::sessionBus().connect(  kService
                                           , kPath
                                           , kInterface
                                           , kName
                                           , this
                                           , SLOT(resReqResult(bool)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    static int i = 0;

    QDBusMessage msg =QDBusMessage::createSignal(kPath, kInterface, kName);
    msg << i;
    QDBusConnection::sessionBus().send(msg);
    i++;
    qDebug() << "on_pushButton_clicked" << i;
}

void MainWindow::on_pushButton_2_clicked()
{

    QDBusInterface* interface = new QDBusInterface(kPath, kInterface);

    //    QDBusPendingReply<int> reply = interface.asyncCall(QStringLiteral("checkIn1"));
    QDBusPendingReply<int> reply = interface->asyncCall(QStringLiteral("checkIn1"));

    reply.waitForFinished();

    qDebug() << "on_pushButton_2_clicked"   << "seconds" << reply.count() << reply.value();
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "on_pushButton_3_clicked";

    QDBusMessage msg = QDBusMessage::createMethodCall(kServiceOn,
                                                          kObj1 ,
                                                          kInterfaceSrv,
                                                          "OnDownloadSlice");
    msg << 4;
    QDBusMessage res = QDBusConnection::sessionBus().call(msg);

    if(QDBusMessage::ReplyMessage == res.type())
    {
        int value {0};
        if(res.arguments().size())
        {
            value = res.arguments().takeFirst().toInt();
        }

        qDebug() << "on_pushButton_3_clicked" << value;
    }

}

void MainWindow::ActiveEvent(int a)
{
    qDebug() << Q_FUNC_INFO << a;
    ui->pushButton->setText(QString::number(a));

}

int MainWindow::ActiveEvent1(int a)
{
    qDebug() << Q_FUNC_INFO << a;
    ui->pushButton_2->setText(QString::number(a));
    return a * a;
}

void MainWindow::resReqResult(bool ok)
{
    qInfo() << Q_FUNC_INFO << ok;
}
