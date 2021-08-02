#include "standarditemergodic.h"

void StandardItemErgodic::ergodicAllItem(QStandardItemModel* model,
                                         callback_ergodicFun_ptr pFun)
{
    if( NULL == model )
        return;

    int rows = model->rowCount();
    int columns = model->columnCount();

    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++)
        {
            QStandardItem* item = model->item(i, j);
            if(item)
            {
                if(!ergodicItem(item, pFun))
                    return;
            }
        }
    }
}

bool StandardItemErgodic::ergodicItem(QStandardItem* item,
                                      callback_ergodicFun_ptr pFun)
{
    if(item)
        return false;

    int rows = item->rowCount();
    int columns = item->columnCount();

    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++)
        {
            QStandardItem* childItem = item->child(i, j);
            if(childItem)
            {
                if(!ergodicItem(childItem, pFun))
                    return false;
            }
        }
    }

    return pFun(item);

}
