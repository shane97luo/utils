#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define READ_SIZE 1024 * 4

#include <QtDBus>

const QString k_INTERFACE = "heygears.interface";

#define kService        "com.service.heygears"
#define kPath           "/com/service/heygears"
#define kInterface      "heygears.interface1"
#define kName           "checkin"

#define kObj1           "/test/objects"
#define kServiceOn      "com.service.heygears.cloud"
#define kInterfaceSrv   "com.service.heygears.cloud.download"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_CLASSINFO("D-Bus Interface", kInterfaceSrv)

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum CupType{
        QW,
        EE,
    };
    Q_ENUM(CupType)

signals:
    void reqResult(bool ok);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void ActiveEvent(int a);

    int ActiveEvent1(int a = 5);

    void resReqResult(bool ok);



private:
    Ui::MainWindow *ui;

    QString model_ = "/home/shane/untitled/build/Ortho Rigid UV_50.ult";

    int file_name_index = 0;

};
#endif // MAINWINDOW_H
