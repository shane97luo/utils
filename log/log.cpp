#include "log.h"
#include "ui_log.h"
#include "hmi.h"
#include "itemdelegate.h"

#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

static const int k_max_log_num = 2000;    // 最大日志数
static const int k_free_log_num = 100;      // 日志满时，每次腾出的空间数

Log::Log(bool bStatic, QWidget *parent) :
    QHView(bStatic, parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);

    QString dbname = "log.db";
    QSqlDatabase log_db = QSqlDatabase::addDatabase("QSQLITE");
    log_db.setDatabaseName(dbname);
    if (!log_db.open()) {
        QMessageBox::critical(0,
                              tr("Cannot open log database"),
                              tr("Unable to establish a database.\n"),
                              QMessageBox::Cancel );

    }

    QSqlQuery query;

    // 主键值自增
    query.exec("create table if not exists Logs (id integer primary key autoincrement, "
               "type interger, time varchar(20), info varchar(100))");

    mModel.reset( new QSqlTableModel(this, log_db) );

    mModel->setTable("Logs");
    mModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    mModel->select();
    mModel->setHeaderData(0, Qt::Horizontal, tr("id"));
    mModel->setHeaderData(1, Qt::Horizontal, tr("type"));
    mModel->setHeaderData(2, Qt::Horizontal, tr("time"));
    mModel->setHeaderData(3, Qt::Horizontal, tr("info"));

    ui->tableView->setModel(mModel.data());
    ui->tableView->hideColumn(0);   //隐藏ID列

    initUI();

    LogStyleDelegate *styleDelegate = new LogStyleDelegate;
    ui->tableView->setItemDelegateForColumn(1, styleDelegate);

}

Log::~Log()
{
    delete ui;

}

void Log::initUI()
{
    ui->pushButton->setText(Tr("logPage", "1", PLUGIN_LANG_RESOURCE));

    ui->pBtnCleanup->setText(Tr("logPage", "4", PLUGIN_LANG_RESOURCE));

    // * Tableview样式
    QString filePath = QDir::currentPath()+"/configs/styles/psblack.qss";
    QFile file(filePath);
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        this->setStyleSheet(qss);
        file.close();
    }
    ui->comboBox->setVisible(false);

#ifdef _TOUCH_SCREEN
    ui->tableView->verticalScrollBar()->setStyleSheet(
                "QScrollBar{background:gray; width: 30px;}");

#if _QT5
    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(35);

#else
    QHeaderView *verticalHeader = ui->tableView->verticalHeader();
    verticalHeader->setResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(35);
#endif

#endif //_TOUCH_SCREEN

    //禁止手动编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

#ifdef _QT5
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(3,QHeaderView::Stretch);
#endif

    ui->tableView->setAlternatingRowColors(true);


}

void Log::on_pBtnCleanup_clicked()
{

    QSqlTableModel *mModel =
            qobject_cast<QSqlTableModel*>(ui->tableView->model());
    if( mModel == 0) return;

    int ret = QMessageBox::warning(
                this,
                Tr("PAGE_SYSTEMLOG","box_title", PLUGIN_LANG_RESOURCE),
                Tr("PAGE_SYSTEMLOG","box_text", PLUGIN_LANG_RESOURCE),
                QMessageBox::Yes, QMessageBox::No);

    // 清空全部记录
    if(ret == QMessageBox::Yes) {
        mModel->removeRows(0, mModel->rowCount());
        mModel->submitAll();

        // FIXME 清空当前日志修改主键序列 sqlite_sequence， 防止一直自增最终溢出
        mModel->setTable("sqlite_sequence");
        QSqlRecord rec = mModel->record(0);
        rec.setValue(1, 99);
        mModel->setRecord(0, rec);
        mModel->submitAll();
        mModel->setTable("Logs");

    }

}

/**
 * @brief Log::insertLogToHead
 * @note  插入一条日志
 * @param type
 * @param Msg
 */
void Log::insertLogToHead(logType type, QString Msg)
{

    QSqlTableModel *mModel =  qobject_cast<QSqlTableModel*>(ui->tableView->model());
    if( mModel == 0) return;

    if(mModel->rowCount()== k_max_log_num)
    {
        mModel->removeRow(k_max_log_num - k_free_log_num);
    }

    QSqlRecord record = mModel->record();
    record.setValue(1, 1);  // type
    record.setValue(2, tr("%1 %2").arg(QDate::currentDate().toString("yyyy-M-dd"))
                    .arg(QTime::currentTime().toString("HH:mm:ss")));   // time
    record.setValue(3, Msg);   // info
    mModel->insertRecord(mModel->rowCount(), record);
    mModel->submitAll();

    // TODO 日志类型设置代理绘制
    switch (type) {
    case INFOTYPE:
        //        mModel->item(0, 0)->setIcon(QIcon(":/image/info"));
        break;
    case TRACETYPE:
        //        mModel->item(0, 0)->setIcon(QIcon(":/image/info"));
        break;
    case WARNTYPE:
        //        mModel->item(0, 0)->setIcon(QIcon(":/image/warn"));
        break;
    case ERRORTYPE:
        //        mModel->item(0, 0)->setIcon(QIcon(":/image/error"));
        break;
    case FATALTYPE:
        //        mModel->item(0, 0)->setIcon(QIcon(":/image/fatal"));
        break;
    default:
        break;
    }

}
