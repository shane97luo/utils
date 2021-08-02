/*************************************************************************
 * Note:        本类设计为单例模式，具有线程安全性
 *
 *
 * Date:        | Change
 * -------------+----------------------
 * 20210716     | 增加数据库及类型筛选
 * 20210726     | support database storage
 * 20210729     | 尝试用线程修改数据库，避免界面卡顿
**************************************************************************/

#ifndef LOG_H
#define LOG_H

#include "qhview.h"
class QSqlTableModel;

namespace Ui {
class Log;
}

enum logType
{
    INFOTYPE    ,
    TRACETYPE   ,
    WARNTYPE    ,
    ERRORTYPE   ,
    FATALTYPE   ,
};

class Log : public QHView
{
    Q_OBJECT

public:
    explicit Log(bool bStatic,QWidget *parent = 0);
    ~Log();

    // prevent copy construct and assignment
    Log &operator =(const Log& rhs) = delete;
    Log(const Log& src) = delete;

protected:

    void initUI();

private:
    Ui::Log *ui;

    QScopedPointer<QSqlTableModel> mModel;

private slots:
    void on_pBtnCleanup_clicked();
    void insertLogToHead(logType lType, QString Msg);

};

#endif // LOG_H
