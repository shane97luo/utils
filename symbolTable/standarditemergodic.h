/*************************************************************************
 *
 * 遍历类
 *
 * 用以处理 QStandardItemModel 的遍历
 * Date:        | Change
 * -------------+----------------------
 * 20210706     |
 *              |
 * 20200420     |
 *
**************************************************************************/


#ifndef STANDARDITEMERGODIC_H
#define STANDARDITEMERGODIC_H

#include <QStandardItemModel>
#include <functional>

struct StandardItemErgodic
{
    // 定义了一个回调函数类型，用以处理取得的 item 对象
    typedef std::function<bool(QStandardItem*)> callback_ergodicFun_ptr;

    static void ergodicAllItem(QStandardItemModel* model, callback_ergodicFun_ptr pFun);
    static bool ergodicItem(QStandardItem* item, callback_ergodicFun_ptr pFun);

};

#endif // STANDARDITEMERGODIC_H
